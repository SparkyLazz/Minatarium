#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "../Characters/Character.h"
//=====================================
//  HELPER FUNCTIONS
//=====================================
static int RollChance(const int chance) {
    if (chance <= 0) return 0;
    if (chance >= 100) return 1;
    return (rand() % 100) < chance;
}

static int ClampPercent(const int value, const int cap) {
    if (value < 0) return 0;
    return (value > cap) ? cap : value;
}

float TotalBlessingEffect(const Character* character, const BlessingEffectType effectType) {
    float totalEffect = 0.0f;

    for (int i = 0; i < character->blessingCount; i++) {
        const Blessing* blessing = &character->currentBlessing[i];

        for (int j = 0; j < blessing->effectsCount; j++) {
            if (blessing->effects[j].type == effectType) {
                totalEffect += blessing->effects[j].baseValue * (float)blessing->stacks;
            }
        }
    }

    return totalEffect;
}

static void CollectDoTEffects(const Character* attacker, DamageResult* result) {
    result->statusCount = 0;

    for (int i = 0; i < attacker->blessingCount; i++) {
        const Blessing* blessing = &attacker->currentBlessing[i];

        for (int j = 0; j < blessing->dotsCount; j++) {
            if (RollChance(blessing->dots[j].change)) {
                if (result->statusCount < 10) {
                    result->appliedStatuses[result->statusCount] = blessing->dots[j].DoT;
                    // Scale DoT damage with blessing stacks
                    result->appliedStatuses[result->statusCount].baseAmount *= (float)blessing->stacks;
                    result->statusCount++;
                }
            }
        }
    }
}

//=====================================
//  DERIVED ATTRIBUTES
//=====================================
// Reads the BASE statistics and the blessing list, and returns the effective
// statistics by value. This must stay free of writes to `character`: the
// previous version stored its results back into the character, so every call
// compounded on the last one and the same blessing was applied over and over.
CombatAttribute ResolveAttributes(const Character* character) {
    CombatAttribute a = character->attribute;

    // Multiplicative on the base value, not on the previous result.
    const float hpBoost = TotalBlessingEffect(character, HP_BOOST);
    a.maxHP = (long long)((float)character->attribute.maxHP * (1.0f + hpBoost));
    if (a.maxHP < 1) a.maxHP = 1;

    const float defBoost = TotalBlessingEffect(character, DEFENSE_BOOST);
    a.defense = (long long)((float)character->attribute.defense * (1.0f + defBoost));
    a.defense += character->tempDefenseBonus;
    if (a.defense < 0) a.defense = 0;

    // Additive on the base value. The generator writes floor-scaled values
    // into the base, so these must add to them rather than replace them.
    a.damageBoost = character->attribute.damageBoost
                  + (int)(TotalBlessingEffect(character, DAMAGE_BOOST) * 100.0f);
    a.criticalChange = ClampPercent(character->attribute.criticalChange
                  + (int)(TotalBlessingEffect(character, CRITICAL_CHANGE) * 100.0f), 100);
    a.criticalDamage = character->attribute.criticalDamage
                  + (int)(TotalBlessingEffect(character, CRITICAL_DAMAGE) * 100.0f);
    a.accuracy = ClampPercent(character->attribute.accuracy
                  + (int)(TotalBlessingEffect(character, ACCURACY_BOOST) * 100.0f), 100);
    a.lifeSteal = ClampPercent(character->attribute.lifeSteal
                  + (int)(TotalBlessingEffect(character, LIFESTEAL) * 100.0f), 100);
    a.regen = ClampPercent(character->attribute.regen
                  + (int)(TotalBlessingEffect(character, REGEN) * 100.0f)
                  + (int)(TotalBlessingEffect(character, REGEN_BOOST) * 100.0f), 25);
    // Hard ceiling of 25%/turn: regeneration must never outpace damage
    // outright, which a stacked REGEN blessing otherwise does trivially.

    // Current health is real mutable state; expose it clamped to the derived
    // maximum so callers never see more than 100% on the bar.
    if (a.hp > a.maxHP) a.hp = a.maxHP;
    if (a.hp < 0) a.hp = 0;

    return a;
}

//=====================================
//  MAIN DAMAGE CALCULATION
//=====================================
DamageResult CalculateDamage(const Character* attacker, const Character* defender) {
    DamageResult result = {0};

    // Derive both sides into locals; neither character is modified.
    const CombatAttribute atk = ResolveAttributes(attacker);
    const CombatAttribute def = ResolveAttributes(defender);

    // 1. Invulnerability is a per-stack chance to negate the attack outright,
    //    capped so it can never become total immunity.
    const float invuln = TotalBlessingEffect(defender, INVULNERABLE);
    if (invuln > 0.0f && RollChance(ClampPercent((int)(invuln * 100.0f), 50))) {
        result.didMiss = 1;
        return result;
    }

    // 2. Check if attack misses. Luck nudges the attacker's effective accuracy.
    const int luck = (int)(TotalBlessingEffect(attacker, LUCK) * 100.0f);
    const int hitChance = ClampPercent(atk.accuracy + luck, 100);
    if (!RollChance(hitChance)) {
        result.didMiss = 1;
        return result;
    }

    // 3. Calculate base damage
    long long baseDamage = atk.attack;

    // 4. Apply damage boost
    const float damageMultiplier = 1.0f + ((float)atk.damageBoost / 100.0f);
    baseDamage = (long long)((float)baseDamage * damageMultiplier);

    // 5. Check for critical hit
    result.isCritical = RollChance(ClampPercent(atk.criticalChange + luck, 100));
    if (result.isCritical) {
        const float critMultiplier = 1.0f + ((float)atk.criticalDamage / 100.0f);
        baseDamage = (long long)((float)baseDamage * critMultiplier);
    }

    result.rawDamage = baseDamage;

    // 6. Apply armor penetration
    float armorPen = TotalBlessingEffect(attacker, ARMOR_PENETRATION);
    if (armorPen > 1.0f) armorPen = 1.0f;
    long long effectiveDefense = (long long)((float)def.defense * (1.0f - armorPen));

    // 7. Shield boost adds flat mitigation on top of defence.
    effectiveDefense += (long long)((float)def.maxHP
                                    * TotalBlessingEffect(defender, SHIELD_BOOST) * 0.01f);

    // 8. Calculate damage after defense
    long long damageAfterDefense = baseDamage - effectiveDefense;
    if (damageAfterDefense < 1) {
        damageAfterDefense = 1; // Minimum 1 damage
    }

    // 9. Apply elemental damage
    const float fireDmg = TotalBlessingEffect(attacker, FIRE_DAMAGE);
    const float iceDmg = TotalBlessingEffect(attacker, ICE_DAMAGE);
    const float poisonDmg = TotalBlessingEffect(attacker, POISON_DAMAGE);

    long long elementalDamage = 0;
    elementalDamage += (long long)((float)baseDamage * fireDmg * (1.0f - (float)def.fireResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * iceDmg * (1.0f - (float)def.iceResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * poisonDmg * (1.0f - (float)def.poisonResistance / 100.0f));
    if (elementalDamage < 0) elementalDamage = 0;

    result.finalDamage = damageAfterDefense + elementalDamage;

    // 10. Calculate lifesteal
    if (atk.lifeSteal > 0) {
        result.lifeStealAmount = (long long)((float)result.finalDamage * ((float)atk.lifeSteal / 100.0f));
    }

    // 11. Check for DoT application
    CollectDoTEffects(attacker, &result);

    // 12. Calculate thorn damage (defender's retaliation)
    const float thornEffect = TotalBlessingEffect(defender, THORN);
    if (thornEffect > 0.0f) {
        result.thornDamage = (long long)((float)result.finalDamage * thornEffect);
    }

    return result;
}

//=====================================
//  APPLY DAMAGE TO TARGET
//=====================================
void ApplyDamageResult(Character* attacker, Character* defender, const DamageResult* result) {
    char logMsg[256];

    if (result->didMiss) {
        sprintf(logMsg, "%s's attack missed!", attacker->name);
        AddCombatLog(logMsg);
        return;
    }

    // Apply damage to defender
    defender->attribute.hp -= result->finalDamage;
    if (defender->attribute.hp < 0) {
        defender->attribute.hp = 0;
    }

    // Display damage info
    if (result->isCritical) {
        sprintf(logMsg, "CRIT! %s dealt %lld dmg to %s",
                attacker->name, result->finalDamage, defender->name);
    } else {
        sprintf(logMsg, "%s dealt %lld damage to %s",
                attacker->name, result->finalDamage, defender->name);
    }
    AddCombatLog(logMsg);

    // Apply lifesteal, capped at the attacker's derived maximum health.
    if (result->lifeStealAmount > 0) {
        const CombatAttribute atk = ResolveAttributes(attacker);
        attacker->attribute.hp += result->lifeStealAmount;
        if (attacker->attribute.hp > atk.maxHP) {
            attacker->attribute.hp = atk.maxHP;
        }
        sprintf(logMsg, "%s healed %lld HP (Lifesteal)",
                attacker->name, result->lifeStealAmount);
        AddCombatLog(logMsg);
    }

    // Apply status effects
    for (int i = 0; i < result->statusCount; i++) {
        CharacterAddStatus(defender, &result->appliedStatuses[i]);

        const char* statusName = "Unknown";
        switch (result->appliedStatuses[i].type) {
            case BURN: statusName = "Burn"; break;
            case POISON: statusName = "Poison"; break;
            case STUN: statusName = "Stun"; break;
            case FREEZE: statusName = "Freeze"; break;
        }
        sprintf(logMsg, "%s is afflicted with %s!", defender->name, statusName);
        AddCombatLog(logMsg);
    }

    // Apply thorn damage
    if (result->thornDamage > 0) {
        attacker->attribute.hp -= result->thornDamage;
        if (attacker->attribute.hp < 0) {
            attacker->attribute.hp = 0;
        }
        sprintf(logMsg, "%s took %lld thorn damage!",
                attacker->name, result->thornDamage);
        AddCombatLog(logMsg);
    }
}

//=====================================
//  STATUS EFFECT TICK
//=====================================
void ProcessStatusEffects(Character* character) {
    char logMsg[256];

    for (int i = 0; i < character->statusCount; i++) {
        Status* status = &character->currentStatus[i];

        const char* statusName = "Unknown";
        switch (status->type) {
            case BURN:
                statusName = "Burn";
                character->attribute.hp -= (long long)status->baseAmount;
                sprintf(logMsg, "%s takes %lld from Burn",
                       character->name, (long long)status->baseAmount);
                AddCombatLog(logMsg);
                break;

            case POISON:
                statusName = "Poison";
                character->attribute.hp -= (long long)status->baseAmount;
                sprintf(logMsg, "%s takes %lld from Poison",
                       character->name, (long long)status->baseAmount);
                AddCombatLog(logMsg);
                break;

            case STUN:
                statusName = "Stun";
                sprintf(logMsg, "%s is stunned!", character->name);
                AddCombatLog(logMsg);
                break;

            case FREEZE:
                statusName = "Freeze";
                sprintf(logMsg, "%s is frozen!", character->name);
                AddCombatLog(logMsg);
                break;
        }

        // Decrement duration
        status->duration--;

        if (status->duration <= 0) {
            sprintf(logMsg, "%s's %s wore off", character->name, statusName);
            AddCombatLog(logMsg);

            // Remove status by shifting array
            for (int j = i; j < character->statusCount - 1; j++) {
                character->currentStatus[j] = character->currentStatus[j + 1];
            }
            character->statusCount--;
            i--; // Adjust index after removal
        }
    }

    if (character->attribute.hp < 0) {
        character->attribute.hp = 0;
    }
}

//=====================================
//  REGENERATION TICK
//=====================================
void ProcessRegeneration(Character* character) {
    const CombatAttribute a = ResolveAttributes(character);

    if (a.regen > 0 && character->attribute.hp < a.maxHP) {
        const long long healAmount = (long long)((float)a.maxHP * ((float)a.regen / 100.0f));
        character->attribute.hp += healAmount;

        if (character->attribute.hp > a.maxHP) {
            character->attribute.hp = a.maxHP;
        }

        char logMsg[256];
        sprintf(logMsg, "%s regenerated %lld HP", character->name, healAmount);
        AddCombatLog(logMsg);
    }
}

//=====================================
//  CHECK IF STUNNED/FROZEN
//=====================================
int IsIncapacitated(const Character* character) {
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == STUN ||
            character->currentStatus[i].type == FREEZE) {
            return 1;
        }
    }
    return 0;
}

//=====================================
//  FULL COMBAT TURN
//=====================================
void ExecuteCombatTurn(Character* attacker, Character* defender) {
    // Check if attacker is incapacitated
    if (IsIncapacitated(attacker)) {
        char logMsg[256];
        sprintf(logMsg, "%s is incapacitated!", attacker->name);
        AddCombatLog(logMsg);
        return;
    }

    // Calculate and apply damage
    const DamageResult result = CalculateDamage(attacker, defender);
    ApplyDamageResult(attacker, defender, &result);
}
