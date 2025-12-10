#ifndef ENEMY_H
#define ENEMY_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"
#define MAX_ENEMY_STATUS 100
#define MAX_ENEMY_BLESSINGS 100
typedef enum {
    ENEMY_NORMAL,
    ENEMY_ELITE,
    ENEMY_BOSS
} EnemyType;
typedef struct {
    char Name[100];

    EnemyType type;
    CombatAttributes attributes;

    //Player Status
    Status currentStatus[MAX_ENEMY_STATUS];
    int statusCount;

    //Player Card
    Blessing currentBlessing[MAX_ENEMY_BLESSINGS];
    int blessingCount;
} Enemy;

Enemy GenerateEnemy(int round);
#endif