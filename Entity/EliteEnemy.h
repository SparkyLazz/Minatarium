#ifndef ELITE_ENEMY_H
#define ELITE_ENEMY_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

#define MAX_ELITE_STATUS 150
#define MAX_ELITE_BLESSINGS 150

typedef struct {
    // Basic Info
    char Name[100];     // Elite enemy name
    int MaxHP;          // Maximum HP
    int HP;             // Current HP
    int Attack;         // Attack power
    int Defense;        // Defense power
    int Level;          // Enemy level
    int Experience;     // EXP reward when defeated
    int Rarity;         // Elite tier (1 = rare, 2 = epic, 3 = legendary)

    // Elite Traits
    int dropRate;       // Chance to drop loot
    int eliteBuff;      // Extra buff multiplier (damage/defense boost)
    int damageMitigation; // Final % damage reduction
    int penetration;    // Ignores part of player defense
    int effectHitRate;  // Chance to apply debuffs/status
    int effectResist;   // Resistance against debuffs/status

    // Status Effects
    Status currentStatus[MAX_ELITE_STATUS];
    int statusCount;

    // Blessings / Traits
    Blessing currentBlessing[MAX_ELITE_BLESSINGS];
    int blessingCount;

    // Special Elite Abilities
    int lifesteal;      // % of damage converted to HP
    int reflectDamage;  // % of damage reflected back to attacker
    int elementalFire;  // Fire damage
    int elementalIce;   // Ice damage (freeze + defense reduction)
    int elementalPoison;// Poison damage (attack reduction)
    int invulnerability;// Temporary immunity to damage

} EliteEnemy;

#endif
