#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "../Characters/Character.h"
#include "../Utils/Utils.h"
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

static void ApplyBlessingAttributeBoosts(Character* character) {
    // Apply HP Boost
    const float hpBoost = CalculateTotalBlessingEffect(character, HP_BOOST);
    character->attribute.maxHP = (long long)((float)character->attribute.maxHP * (1.0f + hpBoost));
    if (character->attribute.hp > character->attribute.maxHP) {
        character->attribute.hp = character->attribute.maxHP;
    }

    // Apply Defense Boost
    const float defBoost = CalculateTotalBlessingEffect(character, DEFENSE_BOOST);
    character->attribute.defense = (long long)((float)character->attribute.defense * (1.0f + defBoost));

    // Apply Damage Boost to damageBoost stat
    const float dmgBoost = CalculateTotalBlessingEffect(character, DAMAGE_BOOST);
    character->attribute.damageBoost = (int)(dmgBoost * 100.0f);

    // Apply Critical Chance
    const float critChance = CalculateTotalBlessingEffect(character, CRITICAL_CHANGE);
    character->attribute.criticalChange += (int)(critChance * 100.0f);
    if (character->attribute.criticalChange > 100) {
        character->attribute.criticalChange = 100;
    }

    // Apply Critical Damage
    const float critDmg = CalculateTotalBlessingEffect(character, CRITICAL_DAMAGE);
    character->attribute.criticalDamage += (int)(critDmg * 100.0f);

    // Apply Accuracy Boost
    const float accBoost = CalculateTotalBlessingEffect(character, ACCURACY_BOOST);
    character->attribute.accuracy += (int)(accBoost * 100.0f);
    if (character->attribute.accuracy > 100) {
        character->attribute.accuracy = 100;
    }

    // Apply Lifesteal
    const float lifesteal = CalculateTotalBlessingEffect(character, LIFESTEAL);
    character->attribute.lifeSteal = (int)(lifesteal * 100.0f);

    // Apply Regen
    const float regen = CalculateTotalBlessingEffect(character, REGEN);
    character->attribute.regen = (int)(regen * 100.0f);
}

//=====================================
//  MAIN DAMAGE CALCULATION
//=====================================
DamageResult CalculateDamage(Character* attacker, Character* defender) {
    DamageResult result = {0};

    // Update attributes based on blessings before calculation
    ApplyBlessingAttributeBoosts(attacker);
    ApplyBlessingAttributeBoosts(defender);

    // 1. Check if attack misses
    const int hitChance = attacker->attribute.accuracy;
    if (!RollChance(hitChance)) {
        result.didMiss = 1;
        return result;
    }

    // 2. Calculate base damage
    long long baseDamage = attacker->attribute.attack;

    // 3. Apply damage boost
    const float damageMultiplier = 1.0f + ((float)attacker->attribute.damageBoost / 100.0f);
    baseDamage = (long long)((float)baseDamage * damageMultiplier);

    // 4. Check for critical hit
    result.isCritical = RollChance(attacker->attribute.criticalChange);
    if (result.isCritical) {
        const float critMultiplier = 1.0f + ((float)attacker->attribute.criticalDamage / 100.0f);
        baseDamage = (long long)((float)baseDamage * critMultiplier);
    }

    result.rawDamage = baseDamage;

    // 5. Apply armor penetration
    const float armorPen = CalculateTotalBlessingEffect(attacker, ARMOR_PENETRATION);
    const long long effectiveDefense = (long long)((float)defender->attribute.defense * (1.0f - armorPen));

    // 6. Calculate damage after defense
    long long damageAfterDefense = baseDamage - effectiveDefense;
    if (damageAfterDefense < 1) {
        damageAfterDefense = 1; // Minimum 1 damage
    }

    // 7. Apply elemental damage
    const float fireDmg = CalculateTotalBlessingEffect(attacker, FIRE_DAMAGE);
    const float iceDmg = CalculateTotalBlessingEffect(attacker, ICE_DAMAGE);
    const float poisonDmg = CalculateTotalBlessingEffect(attacker, POISON_DAMAGE);

    long long elementalDamage = 0;
    elementalDamage += (long long)((float)baseDamage * fireDmg * (1.0f - (float)defender->attribute.fireResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * iceDmg * (1.0f - (float)defender->attribute.iceResistance / 100.0f));
    elementalDamage += (long long)((float)baseDamage * poisonDmg * (1.0f - (float)defender->attribute.poisonResistance / 100.0f));

    result.finalDamage = damageAfterDefense + elementalDamage;

    // 8. Calculate lifesteal
    if (attacker->attribute.lifeSteal > 0) {
        result.lifeStealAmount = (long long)((float)result.finalDamage * ((float)attacker->attribute.lifeSteal / 100.0f));
    }

    // 9. Check for DoT application
    CollectDoTEffects(attacker, &result);

    // 10. Calculate thorn damage (defender's retaliation)
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
    if (result->didMiss) {
        printColor(COL_YELLOW, "%s's attack missed!\n", attacker->name);
        return;
    }

    // Apply damage to defender
    defender->attribute.hp -= result->finalDamage;
    if (defender->attribute.hp < 0) {
        defender->attribute.hp = 0;
    }

    // Display damage info
    if (result->isCritical) {
        printColor(COL_BRIGHT_RED, "CRITICAL HIT! ");
    }
    printColor(COL_RED, "%s dealt %lld damage to %s!\n",
               attacker->name, result->finalDamage, defender->name);

    // Apply lifesteal
    if (result->lifeStealAmount > 0) {
        attacker->attribute.hp += result->lifeStealAmount;
        if (attacker->attribute.hp > attacker->attribute.maxHP) {
            attacker->attribute.hp = attacker->attribute.maxHP;
        }
        printColor(COL_GREEN, "%s healed for %lld HP (Lifesteal)\n",
                   attacker->name, result->lifeStealAmount);
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
        printColor(COL_MAGENTA, "%s is afflicted with %s!\n", defender->name, statusName);
    }

    // Apply thorn damage
    if (result->thornDamage > 0) {
        attacker->attribute.hp -= result->thornDamage;
        if (attacker->attribute.hp < 0) {
            attacker->attribute.hp = 0;
        }
        printColor(COL_CYAN, "%s took %lld thorn damage!\n",
                   attacker->name, result->thornDamage);
    }
}

//=====================================
//  STATUS EFFECT TICK
//=====================================
void ProcessStatusEffects(Character* character) {
    printColor(COL_BRIGHT_BLACK, "\n--- Processing status effects for %s ---\n", character->name);

    for (int i = 0; i < character->statusCount; i++) {
        Status* status = &character->currentStatus[i];

        const char* statusName = "Unknown";
        switch (status->type) {
            case BURN:
                statusName = "Burn";
                character->attribute.hp -= (long long)status->baseAmount;
                printColor(COL_RED, "%s takes %lld damage from Burn!\n",
                           character->name, (long long)status->baseAmount);
                break;

            case POISON:
                statusName = "Poison";
                character->attribute.hp -= (long long)status->baseAmount;
                printColor(COL_GREEN, "%s takes %lld damage from Poison!\n",
                           character->name, (long long)status->baseAmount);
                break;

            case STUN:
                statusName = "Stun";
                printColor(COL_YELLOW, "%s is stunned and cannot act!\n", character->name);
                break;

            case FREEZE:
                statusName = "Freeze";
                printColor(COL_CYAN, "%s is frozen and cannot act!\n", character->name);
                break;
        }

        // Decrement duration
        status->duration--;

        if (status->duration <= 0) {
            printColor(COL_BRIGHT_BLACK, "%s's %s has worn off.\n", character->name, statusName);

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
    if (character->attribute.regen > 0 && character->attribute.hp < character->attribute.maxHP) {
        const long long healAmount = (long long)((float)character->attribute.maxHP *
                                          ((float)character->attribute.regen / 100.0f));
        character->attribute.hp += healAmount;

        if (character->attribute.hp > character->attribute.maxHP) {
            character->attribute.hp = character->attribute.maxHP;
        }

        printColor(COL_GREEN, "%s regenerated %lld HP!\n", character->name, healAmount);
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
        printColor(COL_YELLOW, "%s is incapacitated and cannot attack!\n", attacker->name);
        return;
    }

    // Calculate and apply damage
    const DamageResult result = CalculateDamage(attacker, defender);
    ApplyDamageResult(attacker, defender, &result);
}