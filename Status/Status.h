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
    int duration;
    int maxDuration;
} Status;

#include <stdio.h>

typedef struct {
    // --- Defensive Attributes ---
    int defenseBoost;       // Extra defense points
    int damageReduction;    // Flat or % reduction of incoming damage
    int blockRate;          // Chance to block attacks
    int shield;             // Shield value (absorbs damage)
    int dodge;              // Chance to evade attacks
    int damageMitigation;   // Final % damage reduction after all calculations

    // --- Resistances ---
    int resistanceFire;     // Reduces fire damage/chance
    int resistanceIce;      // Reduces ice damage/chance
    int resistancePoison;   // Reduces poison damage/chance
    int resistanceStun;     // Reduces stun duration/chance
    int resistanceSilence;  // Reduces silence duration/chance
    int resistanceEffect;   // General resistance to debuffs/status effects
    int trueDamageResist;   // Reduces damage that ignores defense

    // --- HP & Recovery ---
    int regen;              // HP regeneration per turn/second
    int regenBoost;         // Boost to healing/regen effects
    int hpBoost;            // Bonus max HP
    int hpAlteration;       // Temporary HP changes (buff/debuff)
    int lifeSteal;          // % of damage converted to HP
    int healBoost;          // Increases healing received

    // --- Debuffs (applied to enemies) ---
    int AR;                 // Attack Reduction (lowers enemy attack)
    int DR;                 // Defense Reduction (lowers enemy defense)

    // --- Status Rates (chance to inflict) ---
    int rateFire;           // Chance to inflict burn
    int rateIce;            // Chance to inflict freeze
    int ratePoison;         // Chance to inflict poison
    int rateStun;           // Chance to inflict stun
    int rateSilence;        // Chance to inflict silence
    int effectHitRate;      // Chance to apply debuffs/status effects

    // --- Elemental Effects ---
    int elementalFire;      // True fire damage (ignores defense)
    int elementalIce;       // Freeze + defense reduction
    int elementalPoison;    // Poison + attack reduction

    // --- Offensive Attributes ---
    int damageBoost;        // Flat/percentage damage increase
    int critChance;         // Chance of critical hit
    int critDamage;         // Extra damage multiplier on crit
    int penetration;        // Ignores part of enemy defense/resistance
    int armorPenetration;   // Ignores armor/defense specifically
    int statusPenetration;  // Ignores enemy effect resistance

    // --- Extra Utility ---
    int accuracy;           // Chance to hit (vs dodge)
    int reflectDamage;      // % of damage reflected back to attacker
    int haste;              // Temporary speed/attack buff
    int slow;               // Temporary debuff applied to enemy
    int invulnerability;    // Temporary immunity to damage
    int silence;            // Temporary unable to cast skill
    int luck;               // Influences random rolls (crit, dodge, loot)
} CombatAttributes;

typedef struct {
    int maxHP;
    int HP;
    int attack;
    int defense;
    CombatAttributes combat;
} Player;


#endif
