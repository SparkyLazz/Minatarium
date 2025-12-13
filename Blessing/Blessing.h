#ifndef BLESSING_H
#define BLESSING_H

#include "../Status/Status.h"
//=====================================
//  RARITY
//=====================================
typedef enum {
    RARITY_COMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY,
} BlessingRarity;

//=====================================
//  EFFECT TYPE
//=====================================
typedef enum {
    DAMAGE_BOOST,
    CRITICAL_CHANGE,
    CRITICAL_DAMAGE,
    ARMOR_PENETRATION,
    ACCURACY_BOOST,

    FIRE_DAMAGE,
    ICE_DAMAGE,
    POISON_DAMAGE,

    HP_BOOST,
    DEFENSE_BOOST,
    SHIELD_BOOST,

    REGEN,
    LIFESTEAL,
    REGEN_BOOST,

    THORN,
    LUCK,
    INVULNERABLE

} BlessingEffectType;

//=====================================
//  BLESSING EFFECT
//=====================================
typedef struct {
    BlessingEffectType type;
    float baseValue;        //0.2 => 20%
} BlessingEffect;

//=====================================
//  BLESSING DOT
//=====================================
typedef struct {
    Status DoT;
    int change;
} BlessingDot;

typedef struct {
    int id;
    char name[64];
    char description[128];

    BlessingRarity rarity;
    long long int stacks;

    BlessingEffect effects[4];
    int effectsCount;

    BlessingDot dots[4];
    int dotsCount;

} Blessing;
#endif