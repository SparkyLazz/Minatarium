#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "Game.h"
#include "../Characters/Character.h"
#include "../Utils/Utils.h"

#define MAX_COMBAT_LOG 10

//=====================================
//  COMBAT LOG SYSTEM
//=====================================
typedef struct {
    char logs[MAX_COMBAT_LOG][256];
    int count;
} CombatLog;

static CombatLog combatLog = {0};

void AddCombatLog(const char* message) {
    if (combatLog.count < MAX_COMBAT_LOG) {
        strncpy(combatLog.logs[combatLog.count], message, 255);
        combatLog.logs[combatLog.count][255] = '\0';
        combatLog.count++;
    } else {
        // Shift logs up and add new one at the end
        for (int i = 0; i < MAX_COMBAT_LOG - 1; i++) {
            strcpy(combatLog.logs[i], combatLog.logs[i + 1]);
        }
        strncpy(combatLog.logs[MAX_COMBAT_LOG - 1], message, 255);
        combatLog.logs[MAX_COMBAT_LOG - 1][255] = '\0';
    }
}

void ClearCombatLog() {
    combatLog.count = 0;
}

//=====================================
//  UI DRAWING FUNCTIONS
//=====================================
void DrawHPBar(const Character* character, const int barWidth) {
    const int filledBars = (int)((float)character->attribute.currentHP / (float)character->attribute.maxHP * (float)barWidth);

    // Color based on HP percentage
    const float hpPercent = (float)character->attribute.currentHP / (float)character->attribute.maxHP;
    Color hpColor;
    if (hpPercent > 0.6f) hpColor = COL_GREEN;
    else if (hpPercent > 0.3f) hpColor = COL_YELLOW;
    else hpColor = COL_RED;

    printColor(hpColor, "[");
    for (int i = 0; i < barWidth; i++) {
        if (i < filledBars) {
            printColor(hpColor, "█");
        } else {
            printColor(COL_BRIGHT_BLACK, "░");
        }
    }
    printColor(hpColor, "]");
}

void DrawStatusIcons(const Character* character) {
    if (character->statusCount == 0) return;

    printf(" [");
    for (int i = 0; i < character->statusCount && i < 5; i++) {
        switch (character->currentStatus[i].type) {
            case BURN: printColor(COL_RED, "F"); break;
            case POISON: printColor(COL_GREEN, "P"); break;
            case STUN: printColor(COL_YELLOW, "S"); break;
            case FREEZE: printColor(COL_CYAN, "I"); break;
        }
    }
    printf("]");
}

void DrawCombatUI(const Character* player, const Character* enemy, const int floor) {
    system("cls");

    // Header
    printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
    printColor(COL_BOLD, "║ ");
    printColor(COL_CYAN, "Floor: %-3d", floor);
    printf("                                            ");

    // Enemy type indicator
    switch (enemy->type) {
        case NORMAL: printColor(COL_GREEN, "[NORMAL]"); break;
        case ELITE: printColor(COL_MAGENTA, "[ELITE]"); break;
        case BOSS: printColor(COL_YELLOW, "[BOSS]"); break;
        default: printf("[     ]");
    }

    printColor(COL_BOLD, " ║\n");
    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

    // Player HP
    printColor(COL_BOLD, "%-20s ", player->name);
    DrawHPBar(player, 30);
    printf(" %lld/%lld", player->attribute.currentHP, player->attribute.maxHP);
    DrawStatusIcons(player);
    if (player->combatState.isDefending) {
        printColor(COL_CYAN, " [DEFEND]");
    }
    printf("\n");

    // Enemy HP
    printColor(COL_BOLD, "%-20s ", enemy->name);
    DrawHPBar(enemy, 30);
    printf(" %lld/%lld", enemy->attribute.currentHP, enemy->attribute.maxHP);
    DrawStatusIcons(enemy);
    if (enemy->combatState.isDefending) {
        printColor(COL_CYAN, " [DEFEND]");
    }
    printf("\n\n");

    // Combat Log
    printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
    printColor(COL_BOLD, "║ ");
    printColor(COL_CYAN, "Combat Log");
    printf("                                                 ");
    printColor(COL_BOLD, "║\n");
    printColor(COL_BOLD, "╠════════════════════════════════════════════════════════════╣\n");

    for (int i = 0; i < combatLog.count; i++) {
        printColor(COL_BOLD, "║ ");
        printf("%-56s", combatLog.logs[i]);
        printColor(COL_BOLD, " ║\n");
    }

    // Fill empty log lines
    for (int i = combatLog.count; i < 8; i++) {
        printColor(COL_BOLD, "║                                                            ║\n");
    }

    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");
}

//=====================================
//  TURN START/END FUNCTIONS
//=====================================
void StartTurn(Character* character) {
    // Reset temporary combat state from previous turn
    ClearCombatState(character);

    // Safety: recalculate stats at start of turn
    RecalculateStats(character);

    // Process status effects
    ProcessStatusEffects(character);
    ProcessRegeneration(character);
}

void EndTurn(Character* character) {
    // Clear defend stance (temporary effect)
    ClearCombatState(character);
}

//=====================================
//  GAME ACTIONS
//=====================================
void PlayerAttackAction(Character* player, Character* enemy) {
    AddCombatLog("=== PLAYER TURN ===");
    ExecuteCombatTurn(player, enemy);

    if (enemy->attribute.currentHP <= 0) {
        char logMsg[256];
        sprintf(logMsg, "%s has been defeated!", enemy->name);
        AddCombatLog(logMsg);
    }
}

void PlayerDefendAction(Character* player) {
    // Set defend STANCE (not stat modification)
    SetDefendingStance(player);

    char logMsg[256];
    sprintf(logMsg, "%s takes a defensive stance! (-%d%% damage)",
            player->name, player->combatState.damageReduction);
    AddCombatLog(logMsg);
}

void EnemyTurn(Character* player, Character* enemy) {
    if (enemy->attribute.currentHP <= 0) return;

    AddCombatLog("=== ENEMY TURN ===");

    // Simple AI: 30% chance to defend if HP < 40%
    float hpPercent = (float)enemy->attribute.currentHP / (float)enemy->attribute.maxHP;
    if (hpPercent < 0.4f && (rand() % 100) < 30) {
        SetDefendingStance(enemy);
        char logMsg[256];
        sprintf(logMsg, "%s takes a defensive stance!", enemy->name);
        AddCombatLog(logMsg);
    } else {
        ExecuteCombatTurn(enemy, player);
    }

    if (player->attribute.currentHP <= 0) {
        char logMsg[256];
        sprintf(logMsg, "%s has been defeated...", player->name);
        AddCombatLog(logMsg);
    }
}

//=====================================
//  COMBAT LOOP
//=====================================
int RunCombat(Character* player, Character* enemy, const int floor) {
    ClearCombatLog();
    AddCombatLog("Combat started!");

    int combatRunning = 1;
    int playerTurn = 1;

    // ReSharper disable once CppDFAConstantConditions
    while (combatRunning) {
        DrawCombatUI(player, enemy, floor);

        if (playerTurn) {
            // START OF PLAYER TURN
            StartTurn(player);

            if (player->attribute.currentHP <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                break;
            }

            if (IsIncapacitated(player)) {
                AddCombatLog("Player is incapacitated!");
                EndTurn(player);
                playerTurn = 0;
                Sleep(1000);
                continue;
            }

            // Player action menu
            printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
            printColor(COL_BOLD, "║ ");
            printColor(COL_GREEN, "[1] Attack");
            printf("      ");
            printColor(COL_BLUE, "[2] Defend");
            printf("                                  ");
            printColor(COL_BOLD, "║\n");
            printColor(COL_BOLD, "║ ");
            printColor(COL_CYAN, "[3] View Player Profile");
            printf("    ");
            printColor(COL_MAGENTA, "[4] View Enemy Profile");
            printf("       ");
            printColor(COL_BOLD, "║\n");
            printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n");
            printf("Your choice: ");

            const int choice = _getch();

            switch (choice) {
                case '1':
                    PlayerAttackAction(player, enemy);
                    EndTurn(player);
                    playerTurn = 0;
                    break;

                case '2':
                    PlayerDefendAction(player);
                    EndTurn(player);
                    playerTurn = 0;
                    break;

                case '3':
                    CharacterRenderer(player);
                    break;

                case '4':
                    CharacterRenderer(enemy);
                    break;

                default:
                    AddCombatLog("Invalid action!");
                    break;
            }

            // Check if enemy is dead
            if (enemy->attribute.currentHP <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                DrawCombatUI(player, enemy, floor);
                break;
            }

        } else {
            // START OF ENEMY TURN
            Sleep(800);
            StartTurn(enemy);

            if (enemy->attribute.currentHP <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                break;
            }

            if (!IsIncapacitated(enemy)) {
                EnemyTurn(player, enemy);
            } else {
                AddCombatLog("Enemy is incapacitated!");
            }

            // END OF ENEMY TURN
            EndTurn(enemy);
            playerTurn = 1;

            // Check if player is dead
            if (player->attribute.currentHP <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                DrawCombatUI(player, enemy, floor);
                break;
            }
        }

        Sleep(500);
    }

    // Return 1 if player won, 0 if player lost
    return player->attribute.currentHP > 0;
}

//=====================================
//  POST-COMBAT REWARDS
//=====================================
void ApplyPostCombatHealing(Character* player) {
    // 10% max HP regen
    const long long baseHeal = player->attribute.maxHP / 10;

    // Additional healing from regen stat
    const long long regenHeal = (long long)((float)player->attribute.maxHP *
                                            ((float)player->attribute.current.regen / 100.0f));

    const long long totalHeal = baseHeal + regenHeal;

    player->attribute.currentHP += totalHeal;
    if (player->attribute.currentHP > player->attribute.maxHP) {
        player->attribute.currentHP = player->attribute.maxHP;
    }

    system("cls");
    printColor(COL_GREEN, "Victory!\n\n");
    printColor(COL_CYAN, "You recovered %lld HP (10%% base + regen bonus)\n", totalHeal);
    printf("Current HP: %lld/%lld\n\n", player->attribute.currentHP, player->attribute.maxHP);

    printf("Press any key to continue...");
    _getch();
}

//=====================================
//  MAIN GAME LOOP
//=====================================
void StartGame() {
    char playerName[100];

    system("cls");
    printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
    printColor(COL_BOLD, "║ ");
    printColor(COL_CYAN, "Enter your name");
    printf("                                          ");
    printColor(COL_BOLD, "║\n");
    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n");
    printf("Name: ");
    fgets(playerName, sizeof(playerName), stdin);
    playerName[strcspn(playerName, "\n")] = 0;

    if (strlen(playerName) == 0) {
        strcpy(playerName, "Adventurer");
    }

    Character player = GeneratePlayer(playerName);
    int currentFloor = 1;
    int gameRunning = 1;

    while (gameRunning) {
        Character enemy = GenerateEnemy(currentFloor);

        const int won = RunCombat(&player, &enemy, currentFloor);

        if (won) {
            // Post-combat healing
            ApplyPostCombatHealing(&player);

            // Give blessing reward
            if (enemy.type == BOSS) {
                const BlessingDatabase* db = GetBlessingDatabase();
                Blessing* legendary = NULL;
                for (int i = 0; i < db->count; i++) {
                    if (db->blessings[i].rarity == RARITY_LEGENDARY) {
                        legendary = &db->blessings[i];
                        break;
                    }
                }
                if (legendary) {
                    BlessingBossReward(&player, legendary);
                }
            } else {
                BlessingWinningReward(&player, enemy.type);
            }

            currentFloor++;

            // Option to continue or quit
            system("cls");
            printColor(COL_BOLD, "Floor %d cleared!\n\n", currentFloor - 1);
            printColor(COL_GREEN, "[1] Continue to Floor %d\n", currentFloor);
            printColor(COL_YELLOW, "[2] View Character\n");
            printColor(COL_RED, "[3] Return to Main Menu\n");
            printf("Choice: ");

            const int choice = _getch();
            if (choice == '2') {
                CharacterRenderer(&player);
            } else if (choice == '3') {
                gameRunning = 0;
            }

        } else {
            // Game over
            system("cls");
            printColor(COL_RED, "╔════════════════════════════════════════════════════════════╗\n");
            printColor(COL_RED, "║                         GAME OVER                          ║\n");
            printColor(COL_RED, "╚════════════════════════════════════════════════════════════╝\n\n");
            printColor(COL_YELLOW, "You were defeated on Floor %d\n\n", currentFloor);
            printf("Press any key to return to main menu...");
            _getch();
            gameRunning = 0;
        }
    }
}