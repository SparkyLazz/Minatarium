#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Blessing/Blessing.h"
#include "../Status/Status.h"
//=====================================
//  CHARACTER ATTRIBUTE
//=====================================
typedef struct{
    long long maxHP;
    long long hp;
    long long attack;
    long long defense;

    int criticalChange;
    int criticalDamage;
    int damageBoost;
    int accuracy;

    int fireDamage;
    int iceDamage;
    int poisonDamage;

    int fireResistance;
    int iceResistance;
    int poisonResistance;

    int lifeSteal;
    int regen;
}CombatAttribute;

//=====================================
//  CHARACTER TYPE
//=====================================
typedef enum {
    PLAYER,
    NORMAL,
    ELITE,
    BOSS
} CharacterType;

//=====================================
//  CHARACTER MAIN
//=====================================
typedef struct {
    char name[100];
    CharacterType type;
    CombatAttribute attribute;

    Blessing currentBlessing[100];
    int blessingCount;

    Status currentStatus[100];
    int statusCount;
} Character;

//=====================================
//  CHARACTER PROPERTY
//=====================================
Character playerBluePrint;

//=====================================
//  CHARACTER RENDERER
//=====================================
void CharacterStatsTab(void* data);
void CharacterBlessingTab(void* data);
void CharacterRenderer(Character* character);
#endif