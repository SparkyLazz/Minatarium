#ifndef SAVE_H
#define SAVE_H

#include "../Characters/Character.h"
#include <time.h>

//=====================================
//  RUN DATA STRUCTURE
//=====================================
typedef struct {
    char playerName[100];
    int floorReached;
    time_t timestamp;

    // Final stats
    long long finalHP;
    long long maxHP;
    long long attack;
    long long defense;
    int criticalChance;
    int criticalDamage;

    // Blessings summary
    int totalBlessings;
    char blessingNames[20][64];  // Store up to 20 blessing names
    long long blessingStacks[20];

} RunData;

//=====================================
//  SAVE SYSTEM FUNCTIONS
//=====================================
void SaveRun(const Character* player, int floorReached);
void LoadAndDisplayRuns();
int GetTotalRunCount();

#endif