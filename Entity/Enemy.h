#ifndef ENEMY_H
#define ENEMY_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

#define MAX_ENEMY_STATUS 100
#define MAX_ENEMY_BLESSINGS 100

typedef struct {

    // Basic Info
    char Name[100];     // Enemy name
    int MaxHP;          // Maximum HP
    int HP;             // Current HP
    int Attack;         // Attack power
    int Defense;        // Defense power
    int Level;          // Enemy level

    // Enemy Status Effects
    Status currentStatus[MAX_ENEMY_STATUS];
    int statusCount;

    // Enemy Blessings / Traits
    Blessing currentBlessing[MAX_ENEMY_BLESSINGS];
    int blessingCount;

} Enemy;

#endif
