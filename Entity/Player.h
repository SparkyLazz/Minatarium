#ifndef PLAYER_H
#define PLAYER_H
#include "../Status/Status.h"
#include "../Blessing/Blessing.h"
#define MAX_PLAYER_STATUS 100
#define MAX_PLAYER_BLESSINGS 100

typedef struct {

    //Status
    char Name[100];

    //Player Attribute
    CombatAttributes attributes;
    //Player Status
    Status currentStatus[MAX_PLAYER_STATUS];
    int statusCount;

    //Player Card
    Blessing currentBlessing[MAX_PLAYER_BLESSINGS];
    int blessingCount;

} Player;

Player CreateDefaultPlayer();
#endif
