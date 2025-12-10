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
    //Common Effect
    BLESS_DAMAGE_BOOST,
    BLESS_HP_BOOST,
    BLESS_DEFEND_BOOST,
    BLESS_CRIT_DAMAGE_BOOST,
    BLESS_CRIT_CHANGE_BOOST,

    //Unique Effect
    BLESS_LIFESTYLE,
    BLESS_THORN,
} BlessingEffectType;

//Bless Char
typedef struct {
    char Name[100];
    char Description[128];
    BlessingEffectType Type;
    BlessingRarity Rarity;

    float valueEffect;
    int maxStackEffect;
    int currentStackEffect;

    StatusType inflictedStatus;
    int applyOnHit; // 0 = no, 1 = try to apply when player hits
    float valueStatus;
    int maxStackStatus;
    int currentStackStatus;
} Blessing;
#endif