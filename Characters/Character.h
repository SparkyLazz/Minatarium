#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Blessing/Blessing.h"
#include "../Status/Status.h"

//=====================================
//  CHARCTER STRUCTURE
//=====================================
typedef struct {
    long long hp;
    long long attack;
    long long defense;
    int criticalChance;
    int criticalDamage;
    int damageBoost;
    int accuracy;
    int fireResistance;
    int iceResistance;
    int poisonResistance;
    int lifeSteal;
    int regen;
} StatBlock;

//=====================================
//  CHARACTER ATTRIBUTE
//=====================================
typedef struct {
    long long maxHP;        // Calculated from base + bonus HP
    long long currentHP;    // Dynamic HP value

    StatBlock base;         // Never changes after creation
    StatBlock bonus;        // From blessings/equipment
    StatBlock current;      // base + bonus (READ-ONLY)
} CombatAttribute;

//=====================================
//  COMBAT STATE
//=====================================
typedef struct {
    int isDefending;
    int damageReduction;    // Percentage (e.g., 50)
    int reflectChance;      // Percentage (e.g., 30)
} CombatState;

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
    CombatState combatState;

    Blessing currentBlessing[100];
    int blessingCount;

    Status currentStatus[100];
    int statusCount;
} Character;

//=====================================
//  STAT MANAGEMENT (CORE FUNCTIONS)
//=====================================
void RecalculateStats(Character* character);
void InitializeCharacterStats(Character* character, StatBlock baseStats, long long baseHP);

//=====================================
//  CHARACTER PROPERTY
//=====================================
extern Character playerBluePrint;
void CharacterAddBlessing(Character* target, const Blessing* blessing);
void CharacterAddStatus(Character* character, const Status* status);

//=====================================
//  COMBAT STATE MANAGEMENT
//=====================================
void SetDefendingStance(Character* character);
void ClearCombatState(Character* character);

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

#endif