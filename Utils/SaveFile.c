#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "Save.h"
#include "../Utils/Utils.h"

#define SAVE_FILE "save.txt"
#define MAX_RUNS_DISPLAY 50

//=====================================
//  SAVE RUN DATA
//=====================================
void SaveRun(const Character* player, const int floorReached) {
    FILE* file = fopen(SAVE_FILE, "a");
    if (file == NULL) {
        printf("Error: Could not open save file for writing!\n");
        return;
    }

    // Write run separator
    fprintf(file, "=== RUN START ===\n");

    // Save basic info
    fprintf(file, "PlayerName: %s\n", player->name);
    fprintf(file, "FloorReached: %d\n", floorReached);
    fprintf(file, "Timestamp: %ld\n", (long)time(NULL));

    // Save final stats
    fprintf(file, "FinalHP: %lld\n", player->attribute.currentHP);
    fprintf(file, "MaxHP: %lld\n", player->attribute.maxHP);
    fprintf(file, "Attack: %lld\n", player->attribute.current.attack);
    fprintf(file, "Defense: %lld\n", player->attribute.current.defense);
    fprintf(file, "CritChance: %d\n", player->attribute.current.criticalChance);
    fprintf(file, "CritDamage: %d\n", player->attribute.current.criticalDamage);
    fprintf(file, "DamageBoost: %d\n", player->attribute.current.damageBoost);
    fprintf(file, "Accuracy: %d\n", player->attribute.current.accuracy);
    fprintf(file, "LifeSteal: %d\n", player->attribute.current.lifeSteal);
    fprintf(file, "Regen: %d\n", player->attribute.current.regen);

    // Save blessing count
    fprintf(file, "BlessingCount: %d\n", player->blessingCount);

    // Save each blessing
    for (int i = 0; i < player->blessingCount; i++) {
        fprintf(file, "Blessing: %s | Stacks: %lld | Rarity: %d\n",
                player->currentBlessing[i].name,
                player->currentBlessing[i].stacks,
                player->currentBlessing[i].rarity);
    }

    fprintf(file, "=== RUN END ===\n\n");

    fclose(file);
}

//=====================================
//  LOAD AND PARSE RUN DATA
//=====================================
static int LoadRunsFromFile(RunData runs[], const int maxRuns) {
    FILE* file = fopen(SAVE_FILE, "r");
    if (file == NULL) {
        return 0;  // No save file exists yet
    }

    int runCount = 0;
    char line[256];
    int inRun = 0;
    RunData currentRun = {0};

    while (fgets(line, sizeof(line), file) != NULL && runCount < maxRuns) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "=== RUN START ===") == 0) {
            inRun = 1;
            memset(&currentRun, 0, sizeof(RunData));
        }
        else if (strcmp(line, "=== RUN END ===") == 0) {
            if (inRun) {
                runs[runCount] = currentRun;
                runCount++;
                inRun = 0;
            }
        }
        else if (inRun) {
            char key[64], value[256];
            if (sscanf(line, "%63[^:]: %255[^\n]", key, value) == 2) {
                if (strcmp(key, "PlayerName") == 0) {
                    strncpy(currentRun.playerName, value, sizeof(currentRun.playerName) - 1);
                }
                else if (strcmp(key, "FloorReached") == 0) {
                    currentRun.floorReached = atoi(value);
                }
                else if (strcmp(key, "Timestamp") == 0) {
                    currentRun.timestamp = (time_t)atol(value);
                }
                else if (strcmp(key, "FinalHP") == 0) {
                    currentRun.finalHP = atoll(value);
                }
                else if (strcmp(key, "MaxHP") == 0) {
                    currentRun.maxHP = atoll(value);
                }
                else if (strcmp(key, "Attack") == 0) {
                    currentRun.attack = atoll(value);
                }
                else if (strcmp(key, "Defense") == 0) {
                    currentRun.defense = atoll(value);
                }
                else if (strcmp(key, "CritChance") == 0) {
                    currentRun.criticalChance = atoi(value);
                }
                else if (strcmp(key, "CritDamage") == 0) {
                    currentRun.criticalDamage = atoi(value);
                }
                else if (strcmp(key, "BlessingCount") == 0) {
                    currentRun.totalBlessings = atoi(value);
                }
                else if (strcmp(key, "Blessing") == 0) {
                    // Parse blessing: "Name | Stacks: X | Rarity: Y"
                    char blessingName[64];
                    long long stacks;
                    if (sscanf(value, "%63[^|] | Stacks: %lld", blessingName, &stacks) == 2) {
                        int idx = 0;
                        while (idx < 20 && strlen(currentRun.blessingNames[idx]) > 0) {
                            idx++;
                        }
                        if (idx < 20) {
                            strncpy(currentRun.blessingNames[idx], blessingName, sizeof(currentRun.blessingNames[idx]) - 1);
                            currentRun.blessingStacks[idx] = stacks;
                        }
                    }
                }
            }
        }
    }

    fclose(file);
    return runCount;
}

//=====================================
//  DISPLAY PREVIOUS RUNS
//=====================================
void LoadAndDisplayRuns() {
    RunData runs[MAX_RUNS_DISPLAY];
    const int runCount = LoadRunsFromFile(runs, MAX_RUNS_DISPLAY);

    if (runCount == 0) {
        system("cls");
        printColor(COL_YELLOW, "No previous runs found!\n\n");
        printf("Press any key to return...");
        _getch();
        return;
    }

    int selectedRun = runCount - 1;  // Start with most recent run
    int viewing = 1;

    while (viewing) {
        system("cls");
        printColor(COL_BOLD, "╔════════════════════════════════════════════════════════════╗\n");
        printColor(COL_BOLD, "║ ");
        printColor(COL_CYAN, "Previous Runs");
        printf(" - Total: %d                              ", runCount);
        printColor(COL_BOLD, "║\n");
        printColor(COL_BOLD, "╚════════════════════════════════════════════════════════════╝\n\n");

        // Display list of runs
        printColor(COL_BOLD, "Run History (most recent first):\n\n");

        int displayStart = selectedRun - 5;
        int displayEnd = selectedRun + 5;
        if (displayStart < 0) displayStart = 0;
        if (displayEnd >= runCount) displayEnd = runCount - 1;

        for (int i = displayEnd; i >= displayStart; i--) {
            if (i == selectedRun) {
                printColor(COL_CYAN, ">>> ");
            } else {
                printf("    ");
            }

            // Format timestamp
            struct tm* timeinfo = localtime(&runs[i].timestamp);
            char timeStr[64];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

            printf("[#%d] %s - Floor %d - %s\n",
                   i + 1, runs[i].playerName, runs[i].floorReached, timeStr);
        }

        printf("\n");
        printColor(COL_BOLD, "════════════════════════════════════════════════════════════\n");
        printColor(COL_BOLD, "Selected Run Details:\n\n");

        RunData* run = &runs[selectedRun];

        printColor(COL_CYAN, "Player Name: ");
        printf("%s\n", run->playerName);
        printColor(COL_YELLOW, "Floor Reached: ");
        printf("%d\n\n", run->floorReached);

        printColor(COL_BOLD, "Final Statistics:\n");
        printf("  HP:            %lld / %lld\n", run->finalHP, run->maxHP);
        printf("  Attack:        %lld\n", run->attack);
        printf("  Defense:       %lld\n", run->defense);
        printf("  Crit Chance:   %d%%\n", run->criticalChance);
        printf("  Crit Damage:   %d%%\n\n", run->criticalDamage);

        if (run->totalBlessings > 0) {
            printColor(COL_BOLD, "Blessings Collected (%d):\n", run->totalBlessings);
            for (int i = 0; i < 20 && strlen(run->blessingNames[i]) > 0; i++) {
                printf("  - %s", run->blessingNames[i]);
                if (run->blessingStacks[i] > 1) {
                    printColor(COL_GREEN, " x%lld\n", run->blessingStacks[i]);
                } else {
                    printf("\n");
                }
            }
        }

        printf("\n");
        printColor(COL_BRIGHT_BLACK, "Use UP/DOWN arrows to navigate, ESC to return\n");

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) { // Up
                selectedRun++;
                if (selectedRun >= runCount) selectedRun = runCount - 1;
            }
            else if (key == 80) { // Down
                selectedRun--;
                if (selectedRun < 0) selectedRun = 0;
            }
        }
        else if (key == 27) { // ESC
            viewing = 0;
        }
    }
}

//=====================================
//  GET TOTAL RUN COUNT
//=====================================
int GetTotalRunCount() {
    RunData runs[MAX_RUNS_DISPLAY];
    return LoadRunsFromFile(runs, MAX_RUNS_DISPLAY);
}