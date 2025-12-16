#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Utils/utils.h"
#include "Characters/Character.h"
#include "Blessing/Blessing.h"
#include "Game/Game.h"

//=====================================
//  ENCYCLOPEDIA FUNCTIONS
//=====================================
void ShowBlessingEncyclopedia() {
    const BlessingDatabase* db = GetBlessingDatabase();
    int selectedIndex = 0;
    int viewing = 1;

    while (viewing) {
        system("cls");
        printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_CYAN, "Blessing Encyclopedia");
        printf("                                    ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

        // Display blessings by rarity
        printColor(COL_BOLD, "Common Blessings:\n");
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_COMMON) {
                if (i == selectedIndex) {
                    printColor(COL_GREEN, ">>> %s\n", db->blessings[i].name);
                } else {
                    printColor(COL_GREEN, "    %s\n", db->blessings[i].name);
                }
            }
        }

        printf("\n");
        printColor(COL_BOLD, "Rare Blessings:\n");
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_RARE) {
                if (i == selectedIndex) {
                    printColor(COL_CYAN, ">>> %s\n", db->blessings[i].name);
                } else {
                    printColor(COL_CYAN, "    %s\n", db->blessings[i].name);
                }
            }
        }

        printf("\n");
        printColor(COL_BOLD, "Epic Blessings:\n");
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_EPIC) {
                if (i == selectedIndex) {
                    printColor(COL_MAGENTA, ">>> %s\n", db->blessings[i].name);
                } else {
                    printColor(COL_MAGENTA, "    %s\n", db->blessings[i].name);
                }
            }
        }

        printf("\n");
        printColor(COL_BOLD, "Legendary Blessings:\n");
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_LEGENDARY) {
                if (i == selectedIndex) {
                    printColor(COL_YELLOW, ">>> %s\n", db->blessings[i].name);
                } else {
                    printColor(COL_YELLOW, "    %s\n", db->blessings[i].name);
                }
            }
        }

        printf("\n");
        printColor(COL_BOLD, "════════════════════════════════════════════════════════════\n");
        printColor(COL_BRIGHT_BLACK, "Press ENTER to view details, UP/DOWN to navigate, ESC to return\n");

        const int key = _getch();
        if (key == 224) {
            const int arrow = _getch();
            if (arrow == 72) { // Up
                selectedIndex--;
                if (selectedIndex < 0) selectedIndex = db->count - 1;
            } else if (arrow == 80) { // Down
                selectedIndex++;
                if (selectedIndex >= db->count) selectedIndex = 0;
            }
        } else if (key == 13) { // Enter - show details
            system("cls");
            Blessing* b = &db->blessings[selectedIndex];

            printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
            printColor(COL_BOLD, "║ ");
            printColor(COL_CYAN, "%s", b->name);
            for (int i = strlen(b->name); i < 54; i++) printf(" ");
            printColor(COL_BOLD, "║\n");
            printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

            printf("Description: %s\n\n", b->description);

            printf("Rarity: ");
            switch (b->rarity) {
                case RARITY_COMMON: printColor(COL_GREEN, "Common\n"); break;
                case RARITY_RARE: printColor(COL_CYAN, "Rare\n"); break;
                case RARITY_EPIC: printColor(COL_MAGENTA, "Epic\n"); break;
                case RARITY_LEGENDARY: printColor(COL_YELLOW, "Legendary\n"); break;
            }

            printf("\nEffects:\n");
            for (int i = 0; i < b->effectsCount; i++) {
                printf("  - ");
                switch (b->effects[i].type) {
                    case DAMAGE_BOOST: printf("Damage Boost"); break;
                    case CRITICAL_CHANGE: printf("Critical Chance"); break;
                    case CRITICAL_DAMAGE: printf("Critical Damage"); break;
                    case ARMOR_PENETRATION: printf("Armor Penetration"); break;
                    case FIRE_DAMAGE: printf("Fire Damage"); break;
                    case ICE_DAMAGE: printf("Ice Damage"); break;
                    case POISON_DAMAGE: printf("Poison Damage"); break;
                    case HP_BOOST: printf("HP Boost"); break;
                    case DEFENSE_BOOST: printf("Defense Boost"); break;
                    case LIFESTEAL: printf("Lifesteal"); break;
                    case REGEN: printf("Regeneration"); break;
                    default: printf("Unknown"); break;
                }
                printf(": %.1f%% per stack\n", b->effects[i].baseValue * 100.0f);
            }

            if (b->dotsCount > 0) {
                printf("\nStatus Effects:\n");
                for (int i = 0; i < b->dotsCount; i++) {
                    printf("  - ");
                    switch (b->dots[i].DoT.type) {
                        case BURN: printColor(COL_RED, "Burn"); break;
                        case POISON: printColor(COL_GREEN, "Poison"); break;
                        case STUN: printColor(COL_YELLOW, "Stun"); break;
                        case FREEZE: printColor(COL_CYAN, "Freeze"); break;
                    }
                    printf(" (%d%% chance, %d turns, %.1f damage)\n",
                           b->dots[i].change,
                           b->dots[i].DoT.duration,
                           b->dots[i].DoT.baseAmount);
                }
            }

            printf("\nPress any key to return...");
            _getch();
        } else if (key == 27) { // ESC
            viewing = 0;
        }
    }
}

void ShowEncyclopedia() {
    int viewing = 1;

    while (viewing) {
        system("cls");
        printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_CYAN, "Encyclopedia");
        printf("                                             ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

        printColor(COL_GREEN, "[1] Blessings\n");
        printColor(COL_CYAN, "[2] Status Effects\n");
        printColor(COL_YELLOW, "[3] Game Mechanics\n");
        printColor(COL_RED, "[4] Return to Main Menu\n\n");
        printf("Choice: ");

        const int choice = _getch();

        switch (choice) {
            case '1':
                ShowBlessingEncyclopedia();
                break;

            case '2':
                system("cls");
                printColor(COL_BOLD, "Status Effects\n\n");
                printColor(COL_RED, "Burn: ");
                printf("Deals damage over time based on attacker's fire damage\n");
                printColor(COL_GREEN, "Poison: ");
                printf("Deals damage over time that ignores defense\n");
                printColor(COL_YELLOW, "Stun: ");
                printf("Prevents the target from taking actions\n");
                printColor(COL_CYAN, "Freeze: ");
                printf("Prevents the target from taking actions\n\n");
                printf("Press any key to return...");
                _getch();
                break;

            case '3':
                system("cls");
                printColor(COL_BOLD, "Game Mechanics\n\n");
                printf("- Defeat enemies to progress through floors\n");
                printf("- Earn blessings after each victory\n");
                printf("- Every 10 floors, face a powerful BOSS\n");
                printf("- Blessings stack for increased power\n");
                printf("- Defend to reduce incoming damage\n");
                printf("- Status effects tick at the start of each turn\n");
                printf("- Heal 10%% HP + Regen bonus after each victory\n\n");
                printf("Press any key to return...");
                _getch();
                break;

            case '4':
                viewing = 0;
                break;
            default: ;
        }
    }
}

//=====================================
//  MAIN MENU
//=====================================
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int running = 1;

    while (running) {
        system("cls");
        printColor(COL_MAGENTA, "$$\\      $$\\ $$\\                      $$\\                         $$\\                         \n");
        printColor(COL_MAGENTA, "$$$\\    $$$ |\\__|                     $$ |                        \\__|                        \n");
        printColor(COL_MAGENTA, "$$$$\\  $$$$ |$$\\ $$$$$$$\\   $$$$$$\\\\ $$$$$$\\    $$$$$$\\   $$$$$$\\  $$\\ $$\\   $$\\ $$$$$$\\\\$$$$\\  \n");
        printColor(COL_CYAN,   "$$\\$$\\$$ $$ |$$ |$$  __$$\\  \\____$$\\\\_$$  _|   \\____$$\\ $$  __$$\\ $$ |$$ |  $$ |$$  _$$  _$$\\ \n");
        printColor(COL_CYAN,   "$$ \\$$$  $$ |$$ |$$ |  $$ | $$$$$$$ | $$ |     $$$$$$$ |$$ |  \\__|$$ |$$ |  $$ |$$ / $$ / $$ |\n");
        printColor(COL_BLUE,   "$$ |\\$  /$$ |$$ |$$ |  $$ |$$  __$$ | $$ |$$\\ $$  __$$ |$$ |      $$ |$$ |  $$ |$$ | $$ | $$ |\n");
        printColor(COL_BLUE,   "$$ | \\_/ $$ |$$ |$$ |  $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$$ |$$ |      $$ |\\$$$$$$  |$$ | $$ | $$ |\n");
        printColor(COL_WHITE,  "\\__|     \\__|\\__|\\__|  \\__| \\_______|  \\____/  \\_______|\\__|      \\__| \\______/ \\__| \\__| \\__|\n\n");

        printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_BRIGHT_BLUE, "[1] Start Game");
        printf("                                            ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_BRIGHT_CYAN, "[2] Encyclopedia");
        printf("                                         ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_BRIGHT_RED, "[3] Quit");
        printf("                                                   ");
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n");
        printColor(COL_WHITE, "\nChoose: ");

        const int choice = _getch();

        switch (choice) {
            case '1':
                StartGame();
                break;

            case '2':
                ShowEncyclopedia();
                break;

            case '3':
                system("cls");
                printColor(COL_CYAN, "Thanks for playing!\n");
                running = 0;
                break;

            default:
                printColor(COL_RED, "Invalid choice!\n");
                break;
        }
    }

    return 0;
}