
#ifndef GAME_H
#define GAME_H

#include "../Status/Status.h"
#include "../Characters/Character.h"
//=====================================
//  BLESSING MANAGER
//=====================================
// Full prototypes: an empty parameter list means "unspecified arguments" in
// C11, which disables argument checking at every call site.
void BlessingWinningReward(Character* player, CharacterType enemyType);
void BlessingBossReward(Character* player, const Blessing* legendaryBlessing);

//=====================================
//  DAMAGE MANAGER
//=====================================
typedef struct {
    long long rawDamage;
    long long finalDamage;
    int isCritical;
    int didMiss;
    long long lifeStealAmount;
    long long thornDamage;
    Status appliedStatuses[10];
    int statusCount;
} DamageResult;

// Pure: folds the character's blessings and turn-scoped bonuses over its BASE
// statistics and returns the result by value. Never mutates the character, so
// calling it any number of times yields the same answer.
CombatAttribute ResolveAttributes(const Character* character);
float TotalBlessingEffect(const Character* character, BlessingEffectType effectType);

DamageResult CalculateDamage(const Character* attacker, const Character* defender);
void ApplyDamageResult(Character* attacker, Character* defender, const DamageResult* result);
void ProcessStatusEffects(Character* character);
void ProcessRegeneration(Character* character);
int IsIncapacitated(const Character* character);
void ExecuteCombatTurn(Character* attacker, Character* defender);

//=====================================
//  GAME LOOP
//=====================================
void StartGame(void);
void AddCombatLog(const char* message);
void ClearCombatLog(void);

//=====================================
//  PLATFORM
//=====================================
// Enables UTF-8 output and ANSI escape interpretation on the Windows console.
void InitConsole(void);
#endif
