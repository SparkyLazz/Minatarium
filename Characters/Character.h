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
void CharacterApplyDamage(Character* target, long long );
void CharacterAddBlessing(Character* target, const Blessing* blessing);
void CharacterAddStatus(Character* character, const Status* status);

//=====================================
//  CHARACTER RENDERER
//=====================================
void CharacterStatsTab(void* data);
void CharacterBlessingTab(void* data);
void CharacterRenderer(Character* character);

//=====================================
//  CHARACTER GENERATOR
//=====================================
void InitRandomGenerator();
CharacterType DetermineEnemyType(int floor);
Character GenerateEnemy(int floor);
Character GeneratePlayer(const char* playerName);
static long long ScaleHP(long long base, int floor, float multiplier);
static long long ScaleLinearStat(long long base, int floor, float multiplier);
static int ScalePercentage(int base, int floor, float multiplier, int cap);
static int CalculateBlessingCount(int floor, CharacterType type);
static long long CalculateBlessingStacks(int floor, CharacterType type);
#endif