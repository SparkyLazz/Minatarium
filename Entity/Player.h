#ifndef PLAYER_H
#define PLAYER_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"
#define MAX_PLAYER_STATUS 100
#define MAX_PLAYER_BLESSINGS 100
typedef struct {

    //Status
    char Name[100];
    int MaxHP;
    int HP;
    int Attack;
    int Defense;

    //Player Status
    Status currentStatus[MAX_PLAYER_STATUS];
    int statusCount;

    //Player Card
    Blessing currentBlessing[MAX_PLAYER_BLESSINGS];
    int blessingCount;

} Player;

#endif