#ifndef STATUS_H
#define STATUS_H

typedef enum {
    STATUS_NONE = 0,
    STATUS_DAMAGE_REDUCTION,
    STATUS_HP_REDUCTION,
} StatusType;

typedef struct {
    StatusType type;
    float value;
    int duration = 1;
    int maxDuration = 4;
} Status;

#include <stdio.h>

typedef struct {
    // --- Defensive Attributes ---
    int defense = 0;
    int defenseBoost = 0;       // Extra defense points
    int damageReduction = 0;    // Flat or % reduction of incoming damage
    int blockRate = 0;          // Chance to block attacks
    int shield = 0;             // Shield value (absorbs damage)
    int dodge = 2;              // Chance to evade attacks
    int damageMitigation = 0;   // Final % damage reduction after all calculations

    // --- Resistances ---
    int resistanceFire = 5;     // Reduces fire damage/chance
    int resistanceIce = 5;      // Reduces ice damage/chance
    int resistancePoison = 5;   // Reduces poison damage/chance
    int resistanceStun = 5;     // Reduces stun duration/chance
    int resistanceSilence = 5;  // Reduces silence duration/chance
    int resistanceEffect = 5;   // General resistance to debuffs/status effects

    // --- HP & Recovery ---
    int hp = 0;
    int regen = 0;              // HP regeneration per turn/second
    int regenBoost = 0;         // Boost to healing/regen effects
    int hpBoost = 0;            // Bonus max HP
    int hpAlteration = 0;       // Temporary HP changes (buff/debuff)
    int lifeSteal = 0;          // % of damage converted to HP
    int healBoost = 0;          // Increases healing received

    // --- Debuffs (applied to enemies) ---
    int AR = 0;                 // Attack Reduction (lowers enemy attack)
    int DR = 0;                 // Defense Reduction (lowers enemy defense)

    // --- Status Rates (chance to inflict) ---
    int rateFire = 0;           // Chance to inflict burn
    int rateIce = 0;            // Chance to inflict freeze
    int ratePoison = 0;         // Chance to inflict poison
    int rateStun = 0;           // Chance to inflict stun
    int rateSilence = 0;        // Chance to inflict silence
    int effectHitRate = 5;      // Chance to apply debuffs/status effects

    // --- Elemental Effects ---
    int elementalFire = 0;      // True fire damage (ignores defense)
    int elementalIce = 0;       // Freeze + defense reduction
    int elementalPoison = 0;    // Poison + attack reduction

    // --- Offensive Attributes ---
    int attack = 0;
    int damageBoost = 0;        // Flat/percentage damage increase
    int critChance = 0;         // Chance of critical hit
    int critDamage = 50;         // Extra damage multiplier on crit
    int penetration = 0;        // Ignores part of enemy defense/resistance
    int armorPenetration = 0;   // Ignores armor/defense specifically
    int statusPenetration = 0;  // Ignores enemy effect resistance

    // --- Extra Utility ---
    int accuracy = 10;           // Chance to hit (vs dodge)
    int reflectDamage = 0;      // % of damage reflected back to attacker
    int invulnerability = 0;    // Temporary immunity to damage
    int silence = 0;            // Temporary unable to cast skill
} CombatAttributes;

typedef struct {
    long long int maxHP = 676767676767676767676767676767;
    long long int HP = 100;
    long long int attack = 10;
    long long int defense = 5;
    CombatAttributes combat;
} Player;


#endif
