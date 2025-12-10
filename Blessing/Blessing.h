#ifndef BLESSING_H
#define BLESSING_H

#include "../Status/Status.h"

// ======================================
//  CONFIG
// ======================================
#define MAX_BLESSING_EFFECTS   4
#define MAX_BLESSING_STATUSES  3

// ======================================
//  RARITY
// ======================================
typedef enum {
    RARITY_COMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
} BlessingRarity;

// ======================================
//  EFFECT TYPES
//  Direct stat modifiers
// ======================================
typedef enum {

    // --- Offensive ---
    BEF_DAMAGE_BOOST,
    BEF_CRIT_CHANCE,
    BEF_CRIT_DAMAGE,
    BEF_ARMOR_PEN,
    BEF_ACCURACY,

    // --- Elemental Offense ---
    BEF_FIRE_DAMAGE,
    BEF_ICE_DAMAGE,
    BEF_POISON_DAMAGE,

    // --- Defensive ---
    BEF_MAX_HP,
    BEF_DEFENSE,
    BEF_DAMAGE_REDUCTION,
    BEF_DODGE,
    BEF_SHIELD,
    BEF_REFLECT,

    // --- Elemental Resistance ---
    BEF_FIRE_RES,
    BEF_ICE_RES,
    BEF_POISON_RES,

    // --- Status Resistance ---
    BEF_RES_STUN,
    BEF_RES_SILENCE,

    // --- Sustain ---
    BEF_REGEN,
    BEF_LIFESTEAL,
    BEF_HEAL_BOOST,

    // --- Unique ---
    BEF_THORN,
    BEF_LUCK,
    BEF_INVULNERABLE,

    // --- Legendary Skill ---
    BEF_GRANT_SKILL

} BlessingEffectType;

// ======================================
//  BLESSING EFFECT
//  - Infinite stack scaling
//  - Used for stat bonuses
// ======================================
typedef struct {
    BlessingEffectType type;
    float baseValue;        // stat bonus per stack
    long long stacks;       // infinite scaling (roguelike)
} BlessingEffect;

// ======================================
//  ON-HIT STATUS EFFECT
//  - Blessing stacks multiply DoT magnitude
//  - Example: magnitude=5, stacks=4 → 20 DoT dmg
// ======================================
typedef struct {
    StatusType type;        // Burn, Poison, Freeze, etc.
    float magnitude;        // base DoT or debuff amount
    int chance;             // % to apply
} BlessingStatus;

// ======================================
//  LEGENDARY SKILL
// ======================================
typedef struct {
    int enabled;
    char name[64];
    int cooldown;
} BlessingSkill;

// ======================================
//  BLESSING STRUCT
// ======================================
typedef struct {

    int id;
    char name[64];
    char description[128];
    BlessingRarity rarity;

    // --- Stat effects ---
    BlessingEffect effects[MAX_BLESSING_EFFECTS];
    int effectCount;

    // --- On-hit effects (DoT, Debuffs, CC) ---
    BlessingStatus statuses[MAX_BLESSING_STATUSES];
    int statusCount;

    // --- Legendary skill ---
    BlessingSkill skill;

} Blessing;

#endif
