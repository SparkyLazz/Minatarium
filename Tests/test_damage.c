// Regression tests for the damage engine.
//
// CalculateDamage is a pure function from two characters to a result struct,
// which makes the properties below cheap to assert. The two most severe
// defects this suite guards against (non-idempotent stat derivation, and
// derived values overwriting generator-supplied base values) both survived in
// the codebase purely because nothing ever called the engine twice and
// compared the answers.
//
// Build and run:  cmake --build <dir> --target MinatariumTests && ctest

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Characters/Character.h"
#include "../Blessing/Blessing.h"
#include "../Game/Game.h"

// Defined in CharacterUtil.c; not currently exposed through a header.
const char* BlessingEffectString(BlessingEffectType e);

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond, ...)                                                   \
    do {                                                                   \
        g_checks++;                                                        \
        if (!(cond)) {                                                     \
            g_failures++;                                                  \
            printf("  FAIL %s:%d: ", __FILE__, __LINE__);                  \
            printf(__VA_ARGS__);                                           \
            printf("\n");                                                  \
        }                                                                  \
    } while (0)

static Character MakeCharacter(const char* name, const long long hp,
                               const long long attack, const long long defense) {
    Character c = {0};
    strncpy(c.name, name, sizeof(c.name) - 1);
    c.type = NORMAL;
    c.attribute.maxHP = hp;
    c.attribute.hp = hp;
    c.attribute.attack = attack;
    c.attribute.defense = defense;
    c.attribute.accuracy = 100;      // deterministic: never miss
    c.attribute.criticalChange = 0;  // deterministic: never crit
    return c;
}

static const Blessing* FindBlessing(const char* name) {
    const BlessingDatabase* db = GetBlessingDatabase();
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->blessings[i].name, name) == 0) return &db->blessings[i];
    }
    return NULL;
}

//=====================================
//  D5: derivation must be idempotent
//=====================================
static void TestResolveIsIdempotent(void) {
    printf("ResolveAttributes is idempotent\n");

    Character c = MakeCharacter("Hero", 1000, 50, 10);
    const Blessing* vitality = FindBlessing("Vitality");     // HP_BOOST 10%
    const Blessing* ironSkin = FindBlessing("Iron Skin");    // DEFENSE_BOOST 5%
    CHECK(vitality && ironSkin, "fixture blessings missing from database");
    if (!vitality || !ironSkin) return;

    CharacterAddBlessing(&c, vitality);
    CharacterAddBlessing(&c, ironSkin);

    const CombatAttribute first = ResolveAttributes(&c);

    // Resolving 50 more times must not move the answer by a single point.
    for (int i = 0; i < 50; i++) {
        const CombatAttribute again = ResolveAttributes(&c);
        CHECK(again.maxHP == first.maxHP,
              "maxHP drifted on call %d: %lld -> %lld", i + 2, first.maxHP, again.maxHP);
        CHECK(again.defense == first.defense,
              "defense drifted on call %d: %lld -> %lld", i + 2, first.defense, again.defense);
        CHECK(again.criticalChange == first.criticalChange,
              "criticalChange drifted on call %d", i + 2);
        if (again.maxHP != first.maxHP) break;   // don't spam 50 identical failures
    }

    // The base record itself must be untouched.
    CHECK(c.attribute.maxHP == 1000, "base maxHP was mutated: %lld", c.attribute.maxHP);
    CHECK(c.attribute.defense == 10, "base defense was mutated: %lld", c.attribute.defense);

    // And the derivation must actually be doing something.
    CHECK(first.maxHP == 1100, "expected 1000 * 1.10 = 1100, got %lld", first.maxHP);
}

//=====================================
//  D5: damage calculation must not mutate its operands
//=====================================
static void TestCalculateDamageIsPure(void) {
    printf("CalculateDamage does not mutate the combatants\n");

    Character atk = MakeCharacter("Attacker", 500, 40, 5);
    Character def = MakeCharacter("Defender", 500, 30, 8);
    const Blessing* vitality = FindBlessing("Vitality");
    if (vitality) CharacterAddBlessing(&def, vitality);

    const CombatAttribute atkBefore = atk.attribute;
    const CombatAttribute defBefore = def.attribute;

    const DamageResult a = CalculateDamage(&atk, &def);
    const DamageResult b = CalculateDamage(&atk, &def);

    CHECK(memcmp(&atkBefore, &atk.attribute, sizeof(CombatAttribute)) == 0,
          "attacker's base attributes changed during damage calculation");
    CHECK(memcmp(&defBefore, &def.attribute, sizeof(CombatAttribute)) == 0,
          "defender's base attributes changed during damage calculation");
    CHECK(a.finalDamage == b.finalDamage,
          "same inputs gave different damage: %lld then %lld", a.finalDamage, b.finalDamage);
}

//=====================================
//  D6: blessings add to generator-supplied base values
//=====================================
static void TestGeneratorValuesSurvive(void) {
    printf("Generator-supplied statistics are not erased by the derivation\n");

    Character e = MakeCharacter("Elite", 800, 60, 20);
    e.attribute.damageBoost = 75;   // what ScalePercentage would have written
    e.attribute.lifeSteal = 20;
    e.attribute.regen = 10;

    CombatAttribute a = ResolveAttributes(&e);
    CHECK(a.damageBoost == 75, "damageBoost erased: expected 75, got %d", a.damageBoost);
    CHECK(a.lifeSteal == 20, "lifeSteal erased: expected 20, got %d", a.lifeSteal);
    CHECK(a.regen == 10, "regen erased: expected 10, got %d", a.regen);

    // Adding a damage blessing must ADD to the scaled value, not replace it.
    const Blessing* might = FindBlessing("Warrior's Might");   // DAMAGE_BOOST 12%
    if (might) {
        CharacterAddBlessing(&e, might);
        a = ResolveAttributes(&e);
        CHECK(a.damageBoost == 87, "expected 75 + 12 = 87, got %d", a.damageBoost);
    }
}

//=====================================
//  D9: status re-application
//=====================================
static void TestStatusStacking(void) {
    printf("Status re-application keeps the stronger magnitude and stays bounded\n");

    Character c = MakeCharacter("Target", 1000, 10, 0);

    const Status weak = { BURN, 5.0f, 3 };
    const Status strong = { BURN, 40.0f, 3 };

    CharacterAddStatus(&c, &weak);
    CharacterAddStatus(&c, &strong);

    CHECK(c.statusCount == 1, "same-type statuses should merge, got %d entries", c.statusCount);
    CHECK(c.currentStatus[0].baseAmount == 40.0f,
          "stronger magnitude was discarded: got %.1f", (double)c.currentStatus[0].baseAmount);

    // Hammer it: duration must not grow without bound (the stun-lock defect).
    for (int i = 0; i < 100; i++) CharacterAddStatus(&c, &strong);
    CHECK(c.currentStatus[0].duration <= 5,
          "duration grew unbounded to %d turns", c.currentStatus[0].duration);
}

//=====================================
//  D7: Defend is turn-scoped
//=====================================
static void TestDefendIsTemporary(void) {
    printf("The Defend bonus is turn-scoped, not permanent\n");

    Character p = MakeCharacter("Hero", 300, 20, 40);
    const long long baseDefense = ResolveAttributes(&p).defense;

    p.tempDefenseBonus = 20;
    CHECK(ResolveAttributes(&p).defense == baseDefense + 20,
          "temporary bonus not applied");

    p.tempDefenseBonus = 0;   // what the start of the next turn does
    CHECK(ResolveAttributes(&p).defense == baseDefense,
          "defence did not return to base after the bonus expired");
    CHECK(p.attribute.defense == 40,
          "base defence was permanently modified: %lld", p.attribute.defense);
}

//=====================================
//  D10/D11/D12: every effect type is reachable and named
//=====================================
static void TestEffectCoverage(void) {
    printf("Every declared effect type is named and granted by some blessing\n");

    const BlessingEffectType all[] = {
        DAMAGE_BOOST, CRITICAL_CHANGE, CRITICAL_DAMAGE, ARMOR_PENETRATION,
        ACCURACY_BOOST, FIRE_DAMAGE, ICE_DAMAGE, POISON_DAMAGE, HP_BOOST,
        DEFENSE_BOOST, SHIELD_BOOST, REGEN, LIFESTEAL, REGEN_BOOST,
        THORN, LUCK, INVULNERABLE
    };
    const int n = (int)(sizeof(all) / sizeof(all[0]));
    const BlessingDatabase* db = GetBlessingDatabase();

    for (int i = 0; i < n; i++) {
        const char* label = BlessingEffectString(all[i]);
        CHECK(strcmp(label, "None") != 0 && strcmp(label, "Unknown") != 0,
              "effect type %d has no display name", (int)all[i]);

        int granted = 0;
        for (int b = 0; b < db->count && !granted; b++) {
            for (int e = 0; e < db->blessings[b].effectsCount; e++) {
                if (db->blessings[b].effects[e].type == all[i]) { granted = 1; break; }
            }
        }
        CHECK(granted, "effect type '%s' is granted by no blessing", label);
    }
}

//=====================================
//  Damage floor
//=====================================
static void TestMinimumDamage(void) {
    printf("An overwhelming defence still takes the minimum of one damage\n");

    Character atk = MakeCharacter("Weak", 100, 1, 0);
    Character def = MakeCharacter("Wall", 100, 1, 1000000);

    const DamageResult r = CalculateDamage(&atk, &def);
    CHECK(!r.didMiss, "accuracy 100 should never miss");
    CHECK(r.finalDamage >= 1, "damage floor breached: %lld", r.finalDamage);
}

int main(void) {
    printf("Minatarium damage engine tests\n");
    printf("==============================\n");

    TestResolveIsIdempotent();
    TestCalculateDamageIsPure();
    TestGeneratorValuesSurvive();
    TestStatusStacking();
    TestDefendIsTemporary();
    TestEffectCoverage();
    TestMinimumDamage();

    printf("==============================\n");
    if (g_failures == 0) {
        printf("PASS: all %d checks passed\n", g_checks);
        return 0;
    }
    printf("FAIL: %d of %d checks failed\n", g_failures, g_checks);
    return 1;
}
