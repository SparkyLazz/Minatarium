#ifndef Entity_Struct_H
#define Entity_Struct_H

typedef struct {
    Character base;         // Inherit base stats
    int experience;         // EXP reward
    int dropRate;           // Loot chance
    int aggression;         // AI behavior
    int attackPattern;      // Defines attack style
} Enemy;

typedef struct {
    Enemy baseEnemy;        // Inherit Enemy fields
    int eliteBuff;          // Extra buff multiplier
} EliteEnemy;

typedef struct {
    int phaseCount;         // Number of combat phases
    int currentPhase;       // Current phase
    int enrageThreshold;    // HP % where boss enrages
    int enrageBoost;        // Buff when enraged
    int ultimateSkill;      // Flag for boss’s strongest attack
} BossEnemy;
