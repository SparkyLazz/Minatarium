#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "../Blessing//Blessing.h"
#include "../Characters/Character.h"
#include "../Utils/Utils.h"
//=====================================
//  GAME REWARD
//=====================================
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
void initRandom() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
}
void DisplayBlessingChoice(Blessing* blessing, const int index, const int selected) {
    if (selected) {
        printColor(COL_BOLD, ">>> ");
    } else {
        printf("    ");
    }

    printColor(COL_BOLD, "[%d] %s\n", index, blessing->name);
    printf("        ");

    // Display rarity with color
    switch (blessing->rarity) {
        case RARITY_COMMON:
            printColor(COL_GREEN, "Common");
            break;
        case RARITY_RARE:
            printColor(COL_CYAN, "Rare");
            break;
        case RARITY_EPIC:
            printColor(COL_MAGENTA, "Epic");
            break;
        case RARITY_LEGENDARY:
            printColor(COL_YELLOW, "Legendary");
            break;
    }

    printf("\n        %s\n", blessing->description);

    // Display effects
    printf("        Effects:\n");
    for (int i = 0; i < blessing->effectsCount; i++) {
        const char* effectName = "Unknown";
        switch (blessing->effects[i].type) {
            case DAMAGE_BOOST: effectName = "Damage Boost"; break;
            case CRITICAL_CHANGE: effectName = "Crit Chance"; break;
            case CRITICAL_DAMAGE: effectName = "Crit Damage"; break;
            case ARMOR_PENETRATION: effectName = "Armor Pen"; break;
            case FIRE_DAMAGE: effectName = "Fire Damage"; break;
            case ICE_DAMAGE: effectName = "Ice Damage"; break;
            case POISON_DAMAGE: effectName = "Poison Damage"; break;
            case HP_BOOST: effectName = "HP Boost"; break;
            case DEFENSE_BOOST: effectName = "Defense Boost"; break;
            case LIFESTEAL: effectName = "Life Steal"; break;
            case REGEN: effectName = "Regeneration"; break;
            default: break;
        }
        printf("          - %s: %.1f%%\n", effectName, blessing->effects[i].baseValue * 100.0f);
    }

    printf("\n");
}
void BlessingWinningReward(Character* player, const CharacterType enemyType) {
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

    Blessing* choice[3];
    for (int i = 0; i < 3; i++) {
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
        printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_CYAN, "Choose Your Blessing");
        printf("                                     ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

        // Display all choices
        for (int i = 0; i < 3; i++) {
            DisplayBlessingChoice(choice[i], i + 1, i == selectedIndex);
        }

        printColor(COL_BRIGHT_BLACK, "Use UP/DOWN arrows to navigate, ENTER to select\n");

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) { // Up
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = 2;
            }
            else if (key == 80) { // Down
                selectedIndex++;
                if (selectedIndex > 2) selectedIndex = 0;
            }
        }
        else if (key == 13) { // Enter
            choosing = 0;
        }
    }

    CharacterAddBlessing(player, choice[selectedIndex]);
    system("cls");

    printColor(COL_GREEN, "You have obtained: ");
    printColor(COL_BOLD, "%s\n", choice[selectedIndex]->name);
    printf("\nPress any key to continue...");
    _getch();
}
void BlessingBossReward(Character* player, Blessing* LegendaryBlessing) {
    int hasLegendary = 0;
    int legendaryIndex = 0;

    for (int i= 0; i < player->blessingCount; i++) {
        if (player->currentBlessing[i].rarity >= RARITY_LEGENDARY) {
            hasLegendary = 1;
            legendaryIndex = i;
            break;
        }
    }

    system("cls");
    printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
    printColor(COL_BOLD, "║ ");
    printColor(COL_YELLOW, "Legendary Blessing Reward");
    printf("                            ");
    printColor(COL_BOLD, "║\n");
    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

    if (!hasLegendary) {
        // No legendary yet, just add it
        printColor(COL_YELLOW, "You have obtained a Legendary Blessing!\n\n");
        DisplayBlessingChoice(LegendaryBlessing, 1, 0);
        CharacterAddBlessing(player, LegendaryBlessing);
        printColor(COL_GREEN, "Legendary blessing has been added to your character!\n");
        printf("\nPress any key to continue...");
        _getch();
        return;
    }

    // Player has legendary, offer replacement
    printColor(COL_YELLOW, "You already have a Legendary Blessing!\n");
    printColor(COL_CYAN, "Legendary blessings cannot be stacked.\n\n");

    printColor(COL_BOLD, "Your current Legendary:\n");
    DisplayBlessingChoice(&player->currentBlessing[legendaryIndex], 1, 0);

    printColor(COL_BOLD, "New Legendary offered:\n");
    DisplayBlessingChoice(LegendaryBlessing, 2, 0);

    printColor(COL_BRIGHT_BLACK, "Do you want to replace your current legendary?\n");
    printColor(COL_GREEN, "[Y] Yes, replace it\n");
    printColor(COL_RED, "[N] No, keep the current one\n");
    printf("\nYour choice: ");

    int choosing = 1;
    while (choosing) {
        const int key = _getch();
        if (key == 'y' || key == 'Y') {
            // Replace the legendary
            player->currentBlessing[legendaryIndex] = *LegendaryBlessing;
            player->currentBlessing[legendaryIndex].stacks = 1;

            system("cls");
            printColor(COL_GREEN, "Legendary blessing has been replaced!\n");
            printf("\nPress any key to continue...");
            _getch();
            choosing = 0;
        } else if (key == 'n' || key == 'N') {
            // Keep the old one
            system("cls");
            printColor(COL_CYAN, "You kept your current legendary blessing.\n");
            printf("\nPress any key to continue...");
            _getch();
            choosing = 0;
        }
    }
}