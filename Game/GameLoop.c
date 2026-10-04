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
//  CONSOLE INITIALISATION
//=====================================
// The UI emits ANSI SGR escape sequences, box-drawing characters and emoji.
// Without these two calls a stock Windows console prints the escapes
// literally and mangles every non-ASCII glyph.
void InitConsole(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (out != INVALID_HANDLE_VALUE && GetConsoleMode(out, &mode)) {
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

//=====================================
//  COMBAT LOG SYSTEM
//=====================================
typedef struct {
    char logs[MAX_COMBAT_LOG][256];
    int count;
    int currentIndex;
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
    combatLog.currentIndex = 0;
}

//=====================================
//  UI DRAWING FUNCTIONS
//=====================================
void DrawHPBar(const Character* character, const int barWidth) {
    // Use the derived maximum: blessings change it, and the bar must be drawn
    // against the same value combat uses.
    const CombatAttribute a = ResolveAttributes(character);
    const long long maxHP = (a.maxHP > 0) ? a.maxHP : 1;

    int filledBars = (int)((float)a.hp / (float)maxHP * (float)barWidth);
    if (filledBars < 0) filledBars = 0;
    if (filledBars > barWidth) filledBars = barWidth;

    // Color based on HP percentage
    const float hpPercent = (float)a.hp / (float)maxHP;
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
            case BURN:
                printColor(COL_RED, "🔥");
                break;
            case POISON:
                printColor(COL_GREEN, "☠");
                break;
            case STUN:
                printColor(COL_YELLOW, "⚡");
                break;
            case FREEZE:
                printColor(COL_CYAN, "❄");
                break;
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
        case PLAYER:
            printf("[     ]");
            break;
        case NORMAL:
            printColor(COL_GREEN, "[NORMAL]");
            break;
        case ELITE:
            printColor(COL_MAGENTA, "[ELITE]");
            break;
        case BOSS:
            printColor(COL_YELLOW, "[BOSS]");
            break;
        default:
            printf("[     ]");
    }

    printColor(COL_BOLD, " ║\n");
    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

    // Player HP
    const CombatAttribute pa = ResolveAttributes(player);
    printColor(COL_BOLD, "%-20.20s ", player->name);
    DrawHPBar(player, 30);
    printf(" %lld/%lld", pa.hp, pa.maxHP);
    DrawStatusIcons(player);
    printf("\n");

    // Enemy HP
    const CombatAttribute ea = ResolveAttributes(enemy);
    printColor(COL_BOLD, "%-20.20s ", enemy->name);
    DrawHPBar(enemy, 30);
    printf(" %lld/%lld", ea.hp, ea.maxHP);
    DrawStatusIcons(enemy);
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

    // Fill empty log lines up to the buffer capacity, so the frame height is
    // constant however many entries are live.
    for (int i = combatLog.count; i < MAX_COMBAT_LOG; i++) {
        printColor(COL_BOLD, "║                                                            ║\n");
    }

    printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");
}

//=====================================
//  GAME ACTIONS
//=====================================
void PlayerAttackAction(Character* player, Character* enemy) {
    char logMsg[256];

    AddCombatLog("=== PLAYER TURN ===");
    ExecuteCombatTurn(player, enemy);

    if (enemy->attribute.hp <= 0) {
        sprintf(logMsg, "%s has been defeated!", enemy->name);
        AddCombatLog(logMsg);
    }
}

void PlayerDefendAction(Character* player) {
    // Defend reduces the next incoming attack. The bonus is turn-scoped: it is
    // written to tempDefenseBonus (cleared at the start of the player's next
    // turn) rather than added to the base defence, which previously made it
    // permanent and stackable on every press.
    const CombatAttribute a = ResolveAttributes(player);
    const long long defenseBoost = (a.defense - player->tempDefenseBonus) / 2 + 1;
    player->tempDefenseBonus = defenseBoost;

    char logMsg[256];
    sprintf(logMsg, "%s takes a defensive stance! (+%lld DEF this turn)",
            player->name, defenseBoost);
    AddCombatLog(logMsg);
}

void EnemyTurn(Character* player, Character* enemy) {
    if (enemy->attribute.hp <= 0) return;

    AddCombatLog("=== ENEMY TURN ===");
    ExecuteCombatTurn(enemy, player);

    if (player->attribute.hp <= 0) {
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

    // Apply blessing effects at start of combat
    char logMsg[256];
    sprintf(logMsg, "Blessing effects applied!");
    AddCombatLog(logMsg);

    int combatRunning = 1;
    int playerTurn = 1;
    // Start-of-turn upkeep must run exactly once per turn. Menu actions that
    // consume no turn (viewing a profile, an unrecognised key) re-enter this
    // loop, and without this guard they re-ticked statuses and regeneration
    // every time: free healing and debuffs that expired while browsing.
    int upkeepDone = 0;

    // ReSharper disable once CppDFAConstantConditions
    while (combatRunning) {
        DrawCombatUI(player, enemy, floor);

        if (playerTurn) {
            if (!upkeepDone) {
                // Last turn's Defend expires now, before the player acts.
                player->tempDefenseBonus = 0;
                ProcessStatusEffects(player);
                ProcessRegeneration(player);
                upkeepDone = 1;

                if (player->attribute.hp <= 0) {
                    combatRunning = 0;
                    break;
                }

                if (IsIncapacitated(player)) {
                    AddCombatLog("Player is incapacitated!");
                    playerTurn = 0;
                    upkeepDone = 0;
                    Sleep(1000);
                    continue;
                }
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
                    playerTurn = 0;
                    upkeepDone = 0;
                    break;

                case '2':
                    PlayerDefendAction(player);
                    playerTurn = 0;
                    upkeepDone = 0;
                    break;

                // These consume no turn, so upkeepDone stays set and the
                // start-of-turn effects are not applied a second time.
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
            if (enemy->attribute.hp <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                DrawCombatUI(player, enemy, floor);
                break;
            }

        } else {
            // Enemy turn
            Sleep(800);
            ProcessStatusEffects(enemy);
            ProcessRegeneration(enemy);

            if (enemy->attribute.hp <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                break;
            }

            if (!IsIncapacitated(enemy)) {
                EnemyTurn(player, enemy);
            } else {
                AddCombatLog("Enemy is incapacitated!");
            }

            playerTurn = 1;
            upkeepDone = 0;

            // Check if player is dead
            if (player->attribute.hp <= 0) {
                // ReSharper disable once CppDFAUnusedValue
                combatRunning = 0;
                DrawCombatUI(player, enemy, floor);
                break;
            }
        }

        Sleep(500);
    }

    // Return 1 if player won, 0 if player lost
    return player->attribute.hp > 0;
}

//=====================================
//  POST-COMBAT REWARDS
//=====================================
void ApplyPostCombatHealing(Character* player) {
    // Heal against the derived maximum, which is what the bar and combat use.
    const CombatAttribute a = ResolveAttributes(player);

    const long long baseHeal = a.maxHP / 10;                                   // 10% of max
    const long long regenHeal = (long long)((float)a.maxHP * ((float)a.regen / 100.0f));
    const long long totalHeal = baseHeal + regenHeal;

    player->attribute.hp += totalHeal;
    if (player->attribute.hp > a.maxHP) {
        player->attribute.hp = a.maxHP;
    }
    player->tempDefenseBonus = 0;   // no Defend bonus carries between fights

    system("cls");
    printColor(COL_GREEN, "Victory!\n\n");
    printColor(COL_CYAN, "You recovered %lld HP (10%% base + regen bonus)\n", totalHeal);
    printf("Current HP: %lld/%lld\n\n", player->attribute.hp, a.maxHP);

    printf("Press any key to continue...");
    _getch();
}

//=====================================
//  MAIN GAME LOOP
//=====================================
void StartGame(void) {
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
    playerName[strcspn(playerName, "\n")] = 0; // Remove newline

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
                // Boss defeated - give a legendary blessing. Picked uniformly
                // at random rather than taking the first match, which made
                // every boss reward the same blessing and left the other
                // legendaries unobtainable.
                const BlessingDatabase* db = GetBlessingDatabase();
                const Blessing* legendary = NULL;
                int seen = 0;
                for (int i = 0; i < db->count; i++) {
                    if (db->blessings[i].rarity == RARITY_LEGENDARY) {
                        seen++;
                        if (rand() % seen == 0) legendary = &db->blessings[i];
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