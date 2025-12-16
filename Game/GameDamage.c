#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static float CalculateTotalBlessingEffect(Character* character, const BlessingEffectType effectType) {
    float totalEffect = 0.0f;

    for (int i = 0; i < character->blessingCount; i++) {
        Blessing* blessing = &character->currentBlessing[i];

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
//  MAIN DAMAGE CALCULATION
//=====================================
DamageResult CalculateDamage(Character* attacker, Character* defender) {
    DamageResult result = {0};

    // 1. Check if attack misses
    const int hitChance = attacker->attribute.current.accuracy;
    if (!RollChance(hitChance)) {
        result.didMiss = 1;
        return result;
    }

    // 2. Calculate base damage (use CURRENT stats)
    long long baseDamage = attacker->attribute.current.attack;

    // 3. Apply damage boost
    const float damageMultiplier = 1.0f + ((float)attacker->attribute.current.damageBoost / 100.0f);
    baseDamage = (long long)((float)baseDamage * damageMultiplier);

    // 4. Check for critical hit (crit chance already capped in RecalculateStats)
    result.isCritical = RollChance(attacker->attribute.current.criticalChance);
    if (result.isCritical) {
        const float critMultiplier = 1.0f + ((float)attacker->attribute.current.criticalDamage / 100.0f);
        baseDamage = (long long)((float)baseDamage * critMultiplier);
    }

    result.rawDamage = baseDamage;

    // 5. Apply armor penetration
    const float armorPen = CalculateTotalBlessingEffect(attacker, ARMOR_PENETRATION);
    long long effectiveDefense = (long long)((float)defender->attribute.current.defense * (1.0f - armorPen));

    // 6. Apply to defend stance (damage reduction, NOT stat modification)
    if (defender->combatState.isDefending) {
        const float reductionMultiplier = 1.0f - ((float)defender->combatState.damageReduction / 100.0f);
        baseDamage = (long long)((float)baseDamage * reductionMultiplier);
    }

    // 7. Calculate damage after defense
    long long damageAfterDefense = baseDamage - effectiveDefense;
    if (damageAfterDefense < 1) {
        damageAfterDefense = 1; // Minimum 1 damage
    }

    // 8. Apply elemental damage
    const float fireDmg = CalculateTotalBlessingEffect(attacker, FIRE_DAMAGE);
    const float iceDmg = CalculateTotalBlessingEffect(attacker, ICE_DAMAGE);
    const float poisonDmg = CalculateTotalBlessingEffect(attacker, POISON_DAMAGE);

    long long elementalDamage = 0;
    elementalDamage += (long long)((float)baseDamage * fireDmg * (1.0f - (float)defender->attribute.current.fireResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * iceDmg * (1.0f - (float)defender->attribute.current.iceResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * poisonDmg * (1.0f - (float)defender->attribute.current.poisonResistance / 100.0f));

    result.finalDamage = damageAfterDefense + elementalDamage;

    // 9. Calculate lifesteal
    if (attacker->attribute.current.lifeSteal > 0) {
        result.lifeStealAmount = (long long)((float)result.finalDamage * ((float)attacker->attribute.current.lifeSteal / 100.0f));
    }

    // 10. Check for DoT application
    CollectDoTEffects(attacker, &result);

    // 11. Calculate thorn damage (defender's retaliation)
    float thornEffect = CalculateTotalBlessingEffect(defender, THORN);
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

    // Apply damage to defender (use currentHP, not stat)
    defender->attribute.currentHP -= result->finalDamage;
    if (defender->attribute.currentHP < 0) {
        defender->attribute.currentHP = 0;
    }

    // Display damage info
    if (result->isCritical) {
        sprintf(logMsg, "CRIT! %s dealt %lld dmg to %s",
                attacker->name, result->finalDamage, defender->name);
    } else {
        sprintf(logMsg, "%s dealt %lld damage to %s",
                attacker->name, result->finalDamage, defender->name);
    }

    if (defender->combatState.isDefending) {
        strcat(logMsg, " (DEFENDED)");
    }
    AddCombatLog(logMsg);

    // Apply lifesteal
    if (result->lifeStealAmount > 0) {
        attacker->attribute.currentHP += result->lifeStealAmount;
        if (attacker->attribute.currentHP > attacker->attribute.maxHP) {
            attacker->attribute.currentHP = attacker->attribute.maxHP;
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
        attacker->attribute.currentHP -= result->thornDamage;
        if (attacker->attribute.currentHP < 0) {
            attacker->attribute.currentHP = 0;
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
                character->attribute.currentHP -= (long long)status->baseAmount;
                sprintf(logMsg, "%s takes %lld from Burn",
                       character->name, (long long)status->baseAmount);
                AddCombatLog(logMsg);
                break;

            case POISON:
                statusName = "Poison";
                character->attribute.currentHP -= (long long)status->baseAmount;
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

    if (character->attribute.currentHP < 0) {
        character->attribute.currentHP = 0;
    }
}

//=====================================
//  REGENERATION TICK
//=====================================
void ProcessRegeneration(Character* character) {
    if (character->attribute.current.regen > 0 && character->attribute.currentHP < character->attribute.maxHP) {
        const long long healAmount = (long long)((float)character->attribute.maxHP *
                                          ((float)character->attribute.current.regen / 100.0f));
        character->attribute.currentHP += healAmount;

        if (character->attribute.currentHP > character->attribute.maxHP) {
            character->attribute.currentHP = character->attribute.maxHP;
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