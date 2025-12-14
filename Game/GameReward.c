#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "../Blessing//Blessing.h"
#include "../Characters/Character.h"
#include "../Utils/Utils.h"
//=====================================
//  GAME REWARD
//=====================================
void initRandom() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
}
Blessing* GetRandomBlessingInRange(const BlessingDatabase* db, const BlessingRarity minRarity, const BlessingRarity maxRarity) {
    int eligibleCount = 0;
    for (int i = 0; i < db->count; i++) {
        if (db->blessings[i].rarity >= minRarity && db->blessings[i].rarity <= maxRarity) {
            eligibleCount++;
        }
    }
    if (eligibleCount == 0) {
        return NULL;
    }

    const int targetIndex = rand() % eligibleCount;
    int currentIndex = 0;

    for (int i = 0; i < db->count; i++) {
        if (db->blessings[i].rarity >= minRarity && db->blessings[i].rarity <= maxRarity) {
            if (currentIndex == targetIndex) {
                return &db->blessings[i];
            }
            currentIndex++;
        }
    }
    return NULL;
}
void BlessingWinningReward(Character* player, CharacterType enemyType) {
    initRandom();
    const BlessingDatabase* db = GetBlessingDatabase();

    BlessingRarity minRarity, maxRarity;
    switch (enemyType) {
        case NORMAL:
            minRarity = RARITY_COMMON;
            maxRarity = RARITY_COMMON;
            break;
        case ELITE:
            minRarity = RARITY_RARE;
            maxRarity = RARITY_RARE;
            break;
        case BOSS:
            minRarity = RARITY_EPIC;
            maxRarity = RARITY_LEGENDARY;
            break;
        default:
            minRarity = RARITY_COMMON;
            maxRarity = RARITY_RARE;
            break;
    }

    for (int i = 0; i < 3; i++) {
        Blessing* choice[3];
        choice[i] = GetRandomBlessingInRange(db, minRarity, maxRarity);
        if (choice[i] == NULL){
            printf("Error: Not enough blessings in database!\n");
            return;
        }
    }

    int selectedIndex = 0;
    int choosing = 1;

    while (choosing) {
        system("cls");
        printColor(COL_BOLD, "=================BLESSING REWARD=================");

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = 2;
            }
            else if (key == 80) {
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
            }
        }
        else if (key == 13) {
            choosing = 0;
        }
    }
}