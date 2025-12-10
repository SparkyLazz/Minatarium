#ifndef PLAYER_H
#define PLAYER_H

#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

// ======================================
// CONFIG
// ======================================
#define MAX_PLAYER_STATUSES   64
#define MAX_PLAYER_BLESSINGS  128

// ======================================
// PLAYER STRUCT
// ======================================
typedef struct {

    char name[64];

    // --- Combat Stats ---
    CombatAttributes baseStats;     // Before blessings
    CombatAttributes finalStats;    // After blessing scaling

    // --- Active Status Effects (Burn, Poison, etc.) ---
    Status statuses[MAX_PLAYER_STATUSES];
    int statusCount;

    // --- Blessings owned ---
    Blessing blessings[MAX_PLAYER_BLESSINGS];
    int blessingCount;

    // --- Extra fields ---
    long long shield;      // Flat absorb shield added by blessings
    int isStunned;         // quick flag for turn logic

} Player;

// ======================================
// FUNCTIONS
// ======================================
Player CreateDefaultPlayer();
void Player_RecalculateStats(Player* p);
void Player_ApplyStatus(Player* p, Status incoming);
void Player_AddBlessing(Player* p, Blessing b);

#endif
