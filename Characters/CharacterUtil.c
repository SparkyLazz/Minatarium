#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Character.h"
#include "../Game/Game.h"
#include "../Utils/Utils.h"

#define LABEL_WIDTH 14
//=====================================
//  CHARACTER PROPERTY
//=====================================
Character playerBluePrint = {
    .name = "",
    .type = PLAYER,
    .attribute = {
        // Balance values, not placeholders. These are commensurate with the
        // enemy scaling curves in this file: a floor-1 normal enemy has 116
        // health and 11 attack.
        .maxHP = 120,
        .hp = 120,
        .attack = 14,
        .defense = 5,
        .criticalChange = 20,
        .criticalDamage = 80,
        .damageBoost = 0,
        .accuracy = 75,
        .fireResistance = 0,
        .iceResistance = 0,
        .poisonResistance = 0,
        .lifeSteal = 0,
        .regen = 0,
    },
    .tempDefenseBonus = 0,
    .blessingCount = 0,
    .statusCount = 0,
};
void CharacterAddBlessing(Character* target, const Blessing* blessing) {
    for (int i = 0; i < target->blessingCount; i++) {
        if (target->currentBlessing[i].id == blessing->id) {
            target->currentBlessing[i].stacks += 1;
            return;
        }
    }

    if (target->blessingCount < 100) {
        target->currentBlessing[target->blessingCount]= *blessing;
        target->currentBlessing[target->blessingCount].stacks = 1;
        target->blessingCount++;
    }
}
// Hard ceiling on how long a single status may persist. Without it, several
// sources of the same status can add duration faster than the one-per-turn
// tick removes it, which locks a stunned or frozen target out permanently.
#define MAX_STATUS_DURATION 5

void CharacterAddStatus(Character* character, const Status* status) {
    for (int i = 0; i < character->statusCount; i++) {
        Status* existing = &character->currentStatus[i];
        if (existing->type == status->type) {
            // Refresh rather than accumulate, and keep the stronger magnitude:
            // a stack-scaled DoT must not be downgraded by a later weak one.
            if (status->baseAmount > existing->baseAmount) {
                existing->baseAmount = status->baseAmount;
            }
            if (status->duration > existing->duration) {
                existing->duration = status->duration;
            }
            if (existing->duration > MAX_STATUS_DURATION) {
                existing->duration = MAX_STATUS_DURATION;
            }
            return;
        }
    }

    if (character->statusCount < 100) {
        character->currentStatus[character->statusCount] = *status;
        if (character->currentStatus[character->statusCount].duration > MAX_STATUS_DURATION) {
            character->currentStatus[character->statusCount].duration = MAX_STATUS_DURATION;
        }
        character->statusCount++;
    }
}
//=====================================
//  CHARACTER RENDERER
//=====================================
void CharacterStatsTab(void* data) {
    const Character* character = (Character*)data;
    // Show the effective statistics (base + blessings), which is what combat
    // actually uses, with the base in parentheses where the two differ.
    const CombatAttribute a = ResolveAttributes(character);
    const CombatAttribute b = character->attribute;

    printColor(COL_BOLD, "Primary Stats\n");
    printf("   HP                : %lld / %lld\n", a.hp, a.maxHP);
    printf("   Attack            : %lld\n", a.attack);
    printf("   Defense           : %lld", a.defense);
    if (a.defense != b.defense) printf("  (base %lld)", b.defense);
    printf("\n\n");

    printColor(COL_BOLD, "Offensive Stats\n");
    printf("   Crit Chance       : %d%%\n", a.criticalChange);
    printf("   Crit Damage       : %d%%\n", a.criticalDamage);
    printf("   Accuracy          : %d%%\n", a.accuracy);
    printf("   Damage Boost      : %d%%\n", a.damageBoost);
    printf("\n");

    printColor(COL_BOLD, "Elemental Stats\n");
    printf("   Fire Resistance   : %d%%\n", a.fireResistance);
    printf("   Ice Resistance    : %d%%\n", a.iceResistance);
    printf("   Poison Resistance : %d%%\n", a.poisonResistance);
    printf("\n");

    printColor(COL_BOLD, "Unique Stats\n");
    printf("   Life Steal        : %d%%\n", a.lifeSteal);
    printf("   Regen             : %d%%\n", a.regen);
    printf("   Thorn             : %.0f%%\n",
           TotalBlessingEffect(character, THORN) * 100.0f);
}
void RarityColor(const BlessingRarity rarity) {
    switch (rarity) {
        case RARITY_COMMON:
            printColor(COL_GREEN, "Common");
            break;
        case RARITY_RARE:
            printColor(COL_CYAN, "Rare");
            break;
        case RARITY_EPIC:
            printColor(COL_MAGENTA, "Epic");
            break;
        case RARITY_LEGENDARY:
            printColor(COL_YELLOW, "Legendary");
    }
}
const char* BlessingEffectString(const BlessingEffectType e) {
    switch (e) {
        case DAMAGE_BOOST: return "Damage Boost";
        case CRITICAL_CHANGE: return "Critical Change";
        case CRITICAL_DAMAGE: return "Critical Damage";
        case ARMOR_PENETRATION: return "Armor Penetration";
        case ACCURACY_BOOST: return "Accuracy Boost";
        case FIRE_DAMAGE: return "Fire Damage";
        case ICE_DAMAGE: return "Ice Damage";
        case POISON_DAMAGE: return "Poison Damage";
        case HP_BOOST: return "HP Boost";
        case DEFENSE_BOOST: return "Defense Boost";
        case SHIELD_BOOST: return "Shield Boost";
        case REGEN: return "Regeneration";
        case LIFESTEAL: return "Life steal";
        case REGEN_BOOST: return "Regen Boost";
        case THORN: return "Thorn";
        case LUCK: return "Luck";
        case INVULNERABLE: return "Invulnerable";
        default: return "None";
    }
}
const char* StatusEffectString(const BlessingDot status) {
    switch (status.DoT.type) {
        case BURN: return "Burn";
        case POISON: return "Poison";
        case STUN: return "Stun";
        case FREEZE: return "Freeze";
        default: return "None";
    }
}
void BlessingStackColor(const long long int stacks) {
    if (stacks <= 10)
        printColor(COL_WHITE, "%lld", stacks);
    else if (stacks <= 30)
        printColor(COL_GREEN, "%lld", stacks);
    else if (stacks <= 50)
        printColor(COL_CYAN, "%lld", stacks);
    else if (stacks <= 75)
        printColor(COL_MAGENTA, "%lld", stacks);
    else if (stacks <= 100)
        printColor(COL_YELLOW, "%lld", stacks);
    else
        printColor(COL_BRIGHT_RED, "%lld", stacks);
}
void BlessingTotalValueColor(const float totalPercent) {
    if (totalPercent < 25.0f)
        printColor(COL_WHITE, "%.2f%%\n", totalPercent);
    else if (totalPercent < 75.0f)
        printColor(COL_GREEN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 150.0f)
        printColor(COL_CYAN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 300.0f)
        printColor(COL_MAGENTA, "%.2f%%\n", totalPercent);
    else
        printColor(COL_YELLOW, "%.2f%%\n", totalPercent);
}
void CharacterBlessingTab(void* data) {
    const Character* character = (Character*)data;
    for (int i = 0; i < character->blessingCount; i++) {
        printColor(COL_BOLD, "[%d] %s\n", i + 1, character->currentBlessing[i].name);
        printf("    %-*s : ", LABEL_WIDTH, "Rarity"); RarityColor(character->currentBlessing[i].rarity);
        printf("\n");
        printf("    %-*s : ", LABEL_WIDTH, "Stacks"); BlessingStackColor(character->currentBlessing[i].stacks);
        printf("\n");
        printf("\n");
        printf("    %-*s : %d\n", LABEL_WIDTH, "Effects",character->currentBlessing[i].effectsCount);
        for (int j = 0; j < character->currentBlessing[i].effectsCount; j++) {
            printf("        %-*s : %s\n",
                   LABEL_WIDTH,
                   "Type",
                   BlessingEffectString(character->currentBlessing[i].effects[j].type));
            printf("        %-*s : %.2f\n",
                   LABEL_WIDTH,
                   "Base Value",
                   character->currentBlessing[i].effects[j].baseValue * 100.0f);
            printf("        %-*s : ",
                   LABEL_WIDTH,
                   "Total Value");
            BlessingTotalValueColor(
                character->currentBlessing[i].effects[j].baseValue *
                (float)character->currentBlessing[i].stacks
            );
            printf("\n");
        }
        if (character->currentBlessing[i].dotsCount > 0) {
            printf("\n");
            printf("    %-*s :\n", LABEL_WIDTH, "On-Hit Statuses");
            for (int j = 0; j < character->currentBlessing[i].dotsCount; j++) {
                printf("        %-*s : %s\n",
                       LABEL_WIDTH,
                       "Type",
                       StatusEffectString(character->currentBlessing[i].dots[j]));
                printf("        %-*s : %d\n",
                       LABEL_WIDTH,
                       "Duration",
                       character->currentBlessing[i].dots[j].DoT.duration);
                printf("        %-*s : %.2f\n",
                       LABEL_WIDTH,
                       "Base Value",
                       character->currentBlessing[i].dots[j].DoT.baseAmount);
                printf("\n");
            }
        }
        printf("\n");
    }
}
void CharacterRenderer(Character* character) {
    Tab tabs[3] = {
        {"Attribute Stat", CharacterStatsTab},
        {"Blessing Stat", CharacterBlessingTab}
    };
    showTabPanel(tabs, 2, character);
}

//=====================================
//  CHARACTER GENERATOR
//=====================================
static long long ScaleHP(const long long base, const int floor, const float multiplier) {
    return (long long)((double)base * pow(1.15, floor * 0.8) * (1.0 + floor * 0.3) * multiplier);
}

static long long ScaleLinearStat(const long long base, const int floor, const float multiplier) {
    return (long long)((double)base * pow(1.12, floor * 0.7) + floor * 2.5 * multiplier);
}

static int ScalePercentage(const int base, const int floor, const float multiplier, const int cap) {
    const int scaled = base + (int)(log2(floor + 1) * 5 * multiplier);
    return (scaled > cap) ? cap : scaled;
}

static int CalculateBlessingCount(const int floor, const CharacterType type) {
    int baseCount = 0;
    switch(type) {
        case PLAYER:
            baseCount = 0;
            break;
        case NORMAL:
            baseCount = (floor / 5);
            break;
        case ELITE:
            baseCount = 1 + (floor / 4);
            break;
        case BOSS:
            baseCount = 2 + (floor / 3);
            break;
        default:
            baseCount = 0;
    }
    return (baseCount > 15) ? 15 : baseCount;
}

static long long CalculateBlessingStacks(const int floor, const CharacterType type) {
    long long stacks;

    switch(type) {
        case PLAYER:
            stacks = 1;
            break;
        case NORMAL:
            // Conservative: 1 + floor/4
            // Floor 1: 1, Floor 10: 3, Floor 40: 11, Floor 100: 26
            stacks = 1 + (floor / 4);
            break;
        case ELITE:
            // Moderate: 1 + floor/3
            // Floor 1: 1, Floor 10: 4, Floor 40: 14, Floor 100: 34
            stacks = 1 + (floor / 3);
            break;
        case BOSS:
            // Aggressive: 1 + floor/2
            // Floor 1: 1, Floor 10: 6, Floor 40: 21, Floor 100: 51
            stacks = 1 + (floor / 2);
            break;
        default:
            stacks = 1;
    }

    return stacks;
}

CharacterType DetermineEnemyType(const int floor) {
    if (floor % 10 == 0) {
        return BOSS;
    }
    if (floor % 7 == 0 || floor % 8 == 0 || floor % 9 == 0) {
        return ELITE;
    }
    return NORMAL;
}

void InitRandomGenerator() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
}

Character GenerateEnemy(const int floor) {
    InitRandomGenerator();

    // Zero-initialise: the struct has fields (tempDefenseBonus, the unused
    // tail of the blessing array) that are otherwise left indeterminate.
    Character enemy = {0};
    const CharacterType type = DetermineEnemyType(floor);
    enemy.type = type;

    // Base stats (floor 1 baseline)
    const long long baseHP = 80;
    const long long baseAttack = 8;
    const long long baseDefense = 3;

    // Type multipliers
    // ReSharper disable once CppDFAUnusedValue
    float hpMult = 1.0f;
    // ReSharper disable once CppDFAUnusedValue
    float atkMult = 1.0f;
    // ReSharper disable once CppDFAUnusedValue
    float defMult = 1.0f;

    switch(type) {
        case PLAYER:   // never generated as an enemy; fall through to Normal
        case NORMAL:
            strcpy(enemy.name, "Normal Enemy");
            hpMult = 1.0f;
            atkMult = 1.0f;
            defMult = 1.0f;
            break;
        case ELITE:
            strcpy(enemy.name, "Elite Enemy");
            hpMult = 1.8f;    // 80% more HP
            atkMult = 1.4f;   // 40% more attack
            defMult = 1.3f;   // 30% more defense
            break;
        case BOSS:
            strcpy(enemy.name, "Boss Enemy");
            hpMult = 3.0f;    // 200% more HP
            atkMult = 1.8f;   // 80% more attack
            defMult = 1.6f;   // 60% more defense
            break;
        // ReSharper disable once CppDFAUnreachableCode
        default:
            strcpy(enemy.name, "Enemy");
    }

    // Apply scaling formulas
    enemy.attribute.maxHP = ScaleHP(baseHP, floor, hpMult);
    enemy.attribute.hp = enemy.attribute.maxHP;
    enemy.attribute.attack = ScaleLinearStat(baseAttack, floor, atkMult);
    enemy.attribute.defense = ScaleLinearStat(baseDefense, floor, defMult);

    // Percentage stats (with caps to maintain balance)
    enemy.attribute.criticalChange = ScalePercentage(5, floor, atkMult, 75);   // Cap at 75%
    enemy.attribute.criticalDamage = ScalePercentage(50, floor, atkMult, 300); // Cap at 300%
    enemy.attribute.damageBoost = ScalePercentage(0, floor, atkMult, 200);     // Cap at 200%
    enemy.attribute.accuracy = ScalePercentage(70, floor, 1.0f, 100);          // Cap at 100%

    // Resistances scale slower
    enemy.attribute.fireResistance = ScalePercentage(0, floor, 0.5f, 50);      // Cap at 50%
    enemy.attribute.iceResistance = ScalePercentage(0, floor, 0.5f, 50);
    enemy.attribute.poisonResistance = ScalePercentage(0, floor, 0.5f, 50);

    // Sustain stats
    // Regeneration heals a PERCENTAGE of maximum health, and maximum health
    // grows exponentially with the floor (ScaleHP), so the amount healed grows
    // exponentially too, while the player's damage grows only linearly in
    // blessing stacks. At the original 20% cap an enemy regenerated 17 HP per
    // turn by floor 5 and 1,257 by floor 20 against a player dealing single
    // digits: every enemy past floor 5 was unkillable. Keep this low enough
    // that regeneration lengthens a fight rather than deciding it.
    enemy.attribute.lifeSteal = ScalePercentage(0, floor, 0.3f, 15);           // Cap at 15%
    enemy.attribute.regen = ScalePercentage(0, floor, 0.15f, 3);               // Cap at 3%

    // Generate blessings
    enemy.blessingCount = CalculateBlessingCount(floor, type);
    const BlessingDatabase* db = GetBlessingDatabase();

    if (enemy.blessingCount > 100) enemy.blessingCount = 100;

    // Count the non-legendary entries once, so the draw below can be a bounded
    // selection instead of a reroll loop that never terminates when the
    // database happens to hold only legendaries.
    int ordinaryCount = 0;
    for (int i = 0; i < db->count; i++) {
        if (db->blessings[i].rarity != RARITY_LEGENDARY) ordinaryCount++;
    }

    // Bosses get 1 guaranteed legendary blessing
    int startIndex = 0;
    if (type == BOSS) {
        // Reservoir-sample one legendary in a single pass: no fixed-size index
        // buffer, so the database can grow without overflowing anything.
        int legendaryCount = 0;
        int chosen = -1;
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_LEGENDARY) {
                legendaryCount++;
                if (rand() % legendaryCount == 0) chosen = i;
            }
        }

        if (chosen >= 0 && enemy.blessingCount < 100) {
            enemy.currentBlessing[0] = db->blessings[chosen];
            enemy.currentBlessing[0].stacks = CalculateBlessingStacks(floor, type);
            startIndex = 1;
            enemy.blessingCount++; // Add 1 for the legendary
        }
    }

    // Generate remaining blessings (non-legendary for variety)
    for (int i = startIndex; i < enemy.blessingCount && i < 100; i++) {
        if (ordinaryCount == 0) {           // nothing eligible to draw
            enemy.blessingCount = i;
            break;
        }
        // Pick the n-th non-legendary entry directly.
        int pick = rand() % ordinaryCount;
        int randomIndex = 0;
        for (int j = 0; j < db->count; j++) {
            if (db->blessings[j].rarity != RARITY_LEGENDARY) {
                if (pick == 0) { randomIndex = j; break; }
                pick--;
            }
        }

        enemy.currentBlessing[i] = db->blessings[randomIndex];

        // Scale stacks based on floor and enemy type
        enemy.currentBlessing[i].stacks = CalculateBlessingStacks(floor, type);
    }

    // No initial status effects
    enemy.statusCount = 0;

    return enemy;
}

Character GeneratePlayer(const char* playerName) {
    Character player = playerBluePrint;

    if (playerName != NULL && strlen(playerName) > 0) {
        strncpy(player.name, playerName, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';
    } else {
        strcpy(player.name, "Player");
    }
    return player;
}