
#ifndef GAME_H
#define GAME_H

#include "../Status/Status.h"
#include "../Characters/Character.h"
//=====================================
//  BLESSING MANAGER
//=====================================
void BlessingWinningReward();
void BlessingBossReward();

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

DamageResult CalculateDamage(Character* attacker, Character* defender);
void ApplyDamageResult(Character* attacker, Character* defender, const DamageResult* result);
void ProcessStatusEffects(Character* character);
void ProcessRegeneration(Character* character);
int IsIncapacitated(const Character* character);
void ExecuteCombatTurn(Character* attacker, Character* defender);
#endif