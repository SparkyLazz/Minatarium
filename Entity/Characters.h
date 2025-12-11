#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

#define MAX_BLESSING 100
#define MAX_STATUS   100
#define MAX_SKILLS   100

typedef enum {
    CHARACTER_PLAYER = 0,
    CHARACTER_NORMAL,
    CHARACTER_ELITE,
    CHARACTER_BOSS
} CharacterType;

typedef struct Characters {
    char name[100];
    CharacterType type;
    CombatAttributes attributes;

    Blessing currentBlessing[MAX_BLESSING];
    int blessingCount;

    Status currentStatus[MAX_STATUS];
    int statusCount;

    BlessingSkill currentSkill[MAX_SKILLS];
    int skillCount;

} Characters;

#endif
