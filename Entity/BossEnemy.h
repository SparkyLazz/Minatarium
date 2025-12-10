#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

#define MAX_BOSS_STATUS 200
#define MAX_BOSS_BLESSINGS 200

typedef struct {
    // --- Basic Info ---
    char Name[100];     // Boss name
    int MaxHP;          // Maximum HP
    int HP;             // Current HP
    int Attack;         // Attack power
    int Defense;        // Defense power
    int Level;          // Boss level

    // --- Boss Traits ---
    int phaseCount;     // Number of combat phases
    int currentPhase;   // Current phase in battle
    int enrageThreshold;// HP % where boss enrages
    int enrageBoost;    // Damage/defense boost when enraged
    int attackPattern;  // Defines attack style/pattern
    int dropRate;       // Loot drop chance

    // --- Status Effects ---
    Status currentStatus[MAX_BOSS_STATUS];
    int statusCount;

    // --- Blessings / Traits ---
    Blessing currentBlessing[MAX_BOSS_BLESSINGS];
    int blessingCount;

    // --- Special Boss Abilities ---
    int lifesteal;        // % of damage converted to HP
    int reflectDamage;    // % of damage reflected back
    int elementalFire;    // Fire damage
    int elementalIce;     // Ice damage (freeze + defense reduction)
    int elementalPoison;  // Poison damage (attack reduction)
    int invulnerability;  // Temporary immunity to damage
    int ultimateSkill;    // Flag for boss’s strongest attack
    int damageMitigation; // Final % damage reduction
    int penetration;      // Ignores part of player defense
    int effectHitRate;    // Chance to apply debuffs/status
    int effectResist;     // Resistance against debuffs/status
} BossEnemy;

#endif
