#ifndef BLESSING_H
#define BLESSING_H
#include "../Status/Status.h"

//Blessing Rareness
typedef enum {
    RARITY_COMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
} BlessingRarity;

//Bless Effect Type
typedef enum {
    // --- Common Effects ---
    BLESS_DAMAGE_BOOST,
    BLESS_HP_BOOST,
    BLESS_DEFEND_BOOST,
    BLESS_CRIT_DAMAGE_BOOST,
    BLESS_CRIT_CHANCE_BOOST,

    // --- Offensive Effects ---
    BLESS_ATTACK_SPEED,        // faster attacks
    BLESS_ACCURACY_BOOST,      // better chance to hit
    BLESS_ELEMENTAL_FIRE,      // adds fire damage
    BLESS_ELEMENTAL_ICE,       // adds ice damage
    BLESS_ELEMENTAL_POISON,    // adds poison damage
    BLESS_ARMOR_PENETRATION,   // ignores part of enemy defense

    // --- Defensive Effects ---
    BLESS_SHIELD,              // adds shield value
    BLESS_DODGE_BOOST,         // increases dodge chance
    BLESS_DAMAGE_REDUCTION,    // reduces incoming damage
    BLESS_REFLECT_DAMAGE,      // reflects damage back to attacker
    BLESS_RESISTANCE_FIRE,
    BLESS_RESISTANCE_ICE,
    BLESS_RESISTANCE_POISON,
    BLESS_RESISTANCE_STUN,
    BLESS_RESISTANCE_SILENCE,

    // --- Recovery & Sustain ---
    BLESS_REGEN_HP,            // passive HP regen
    BLESS_HEAL_BOOST,          // increases healing received
    BLESS_LIFESTEAL,           // heals from damage dealt

    // --- Utility / Unique Effects ---
    BLESS_THORN,               // deals damage when hit
    BLESS_LUCK_BOOST,          // improves crit/loot rolls
    BLESS_INVULNERABILITY,     // temporary immunity
    BLESS_HASTE,               // temporary speed buff
    BLESS_SLOW,                // applies slow to enemies
    BLESS_SUMMON,              // summon minions or allies
    BLESS_ULTIMATE,            // unlocks ultimate skill (Boss ability)

    // Legendary (Skill-giving)
    BLESS_GRANT_SKILL
} BlessingEffectType;

//Bless Char
typedef struct {
    char Name[100];
    char Description[128];
    BlessingEffectType Type;
    BlessingRarity Rarity;

    //Effect Scaling
    float valueEffect;
    int maxStackEffect;
    int currentStackEffect;

    StatusType inflictedStatus;
    int applyOnHit; // 0 = no, 1 = try to apply when player hits
    float valueStatus;
    int maxStackStatus;
    int currentStackStatus;

    // LEGENDARY ONLY → gives special action
    int grantsSkill;
    char SkillName[100];
    int SkillCooldown;
    int SkillCurrentCD;

} Blessing;
#endif