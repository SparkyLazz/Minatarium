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
// `attribute` holds the BASE statistics only: the values written once by the
// generator. Everything blessings modify is derived on demand by
// ResolveAttributes() and never written back here, so the derivation stays
// idempotent no matter how often it runs. The single exception is
// `attribute.hp`, which is genuine per-turn mutable state.
typedef struct {
    char name[100];
    CharacterType type;
    CombatAttribute attribute;

    // Turn-scoped defence bonus (the Defend action). Cleared at the start of
    // the owner's next turn; never folded into the base statistics.
    long long tempDefenseBonus;

    Blessing currentBlessing[100];
    int blessingCount;

    Status currentStatus[100];
    int statusCount;
} Character;

//=====================================
//  CHARACTER PROPERTY
//=====================================
extern Character playerBluePrint;
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
void InitRandomGenerator(void);
CharacterType DetermineEnemyType(int floor);
Character GenerateEnemy(int floor);
Character GeneratePlayer(const char* playerName);
// The scaling helpers are static to CharacterUtil.c and are deliberately not
// declared here: a `static` declaration in a shared header promises a
// definition in every translation unit that includes it.
#endif