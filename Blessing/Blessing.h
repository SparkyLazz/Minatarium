#ifndef BLESSING_H
#define BLESSING_H

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

    float value;
    int maxStack;
    int currentStack;
} Blessing;
#endif