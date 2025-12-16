# Utils Module Documentation

## 📋 Overview

The Utils module provides cross-cutting functionality used throughout the game, including colored terminal output, UI panels, and persistent save system. These utilities enhance the user experience and enable data persistence.

## 🎯 Design Philosophy

1. **Reusability**: Functions used by multiple modules
2. **Abstraction**: Hide platform-specific details
3. **Simplicity**: Easy-to-use interfaces
4. **Consistency**: Uniform styling and behavior

## 📁 File Structure

```
Utils/
├── Utils.h       # Interface for printing and panels
├── PrintUtils.c  # Colored terminal output
├── PanelUtil.c   # Tab-based UI system
├── Save.h        # Save system interface
└── SaveFile.c    # File persistence implementation
```

## 🎨 Print Utilities

### Color System

The print utilities provide ANSI color code support for Windows console.

#### Color Enum

```c
typedef enum {
    // Normal Foreground Colors
    COL_RESET,
    COL_BLACK,
    COL_RED,
    COL_GREEN,
    COL_YELLOW,
    COL_BLUE,
    COL_MAGENTA,
    COL_CYAN,
    COL_WHITE,

    // Bright Foreground Colors
    COL_BRIGHT_BLACK,
    COL_BRIGHT_RED,
    COL_BRIGHT_GREEN,
    COL_BRIGHT_YELLOW,
    COL_BRIGHT_BLUE,
    COL_BRIGHT_MAGENTA,
    COL_BRIGHT_CYAN,
    COL_BRIGHT_WHITE,

    // Background Colors (Normal)
    COL_BG_BLACK,
    COL_BG_RED,
    COL_BG_GREEN,
    COL_BG_YELLOW,
    COL_BG_BLUE,
    COL_BG_MAGENTA,
    COL_BG_CYAN,
    COL_BG_WHITE,

    // Background Colors (Bright)
    COL_BG_BRIGHT_BLACK,
    COL_BG_BRIGHT_RED,
    COL_BG_BRIGHT_GREEN,
    COL_BG_BRIGHT_YELLOW,
    COL_BG_BRIGHT_BLUE,
    COL_BG_BRIGHT_MAGENTA,
    COL_BG_BRIGHT_CYAN,
    COL_BG_BRIGHT_WHITE,

    // Text Styles
    COL_BOLD,
    COL_UNDERLINE,

    COL_MAX_ENUM_VALUE  // Sentinel for safety checks
} Color;
```

#### Color Code Mapping

```c
static const char* COLOR_CODES[] = {
    [COL_RESET]            = "\033[0m",
    [COL_BLACK]            = "\033[30m",
    [COL_RED]              = "\033[31m",
    [COL_GREEN]            = "\033[32m",
    [COL_YELLOW]           = "\033[33m",
    // ... full mapping in PrintUtils.c
};
```

#### Core Function

```c
void printColor(const Color color, const char* format, ...) {
    va_list args;
    
    // Safety check: prevent out-of-bounds access
    Color safeColor = color;
    if (safeColor >= COL_MAX_ENUM_VALUE)
        safeColor = COL_RESET;
    
    // Apply color
    printf("%s", COLOR_CODES[safeColor]);
    
    // Print formatted text (supports printf-style formatting)
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    // Reset color
    printf("%s", COLOR_CODES[COL_RESET]);
}
```

### Usage Examples

#### Simple Colored Text

```c
printColor(COL_RED, "Error occurred!\n");
printColor(COL_GREEN, "Success!\n");
printColor(COL_YELLOW, "Warning: Low HP\n");
```

#### Formatted Output

```c
int playerHP = 450;
int maxHP = 600;
printColor(COL_CYAN, "HP: %d/%d\n", playerHP, maxHP);

float damagePercent = 85.5f;
printColor(COL_BOLD, "Critical Hit! %.1f%% damage\n", damagePercent);
```

#### Combined Styles

```c
// Bold + colored text
printColor(COL_BOLD, "");
printColor(COL_RED, "GAME OVER");
printColor(COL_RESET, "\n");
```

### Color Usage Guide

| Color | Usage | Example |
|-------|-------|---------|
| RED | Errors, danger, burn | "GAME OVER", burn damage |
| GREEN | Success, health, common | "Victory!", HP bar (high HP) |
| YELLOW | Warning, legendary | "Low HP!", legendary blessings |
| BLUE | Info, ice effects | "Floor cleared", freeze status |
| CYAN | Highlights, rare | Player name, rare blessings |
| MAGENTA | Special, epic | Elite enemies, epic blessings |
| WHITE | Normal text | Default message text |
| BRIGHT_BLACK | Subtle info | Help text, borders |

## 🖼️ Panel Utilities

### Tab Panel System

The panel system provides an interactive tabbed interface for viewing complex data.

#### Tab Structure

```c
typedef struct {
    char title[64];                      // Tab label
    void (*renderContent)(void* data);   // Function to render tab content
} Tab;
```

#### Core Function

```c
void showTabPanel(Tab tabs[], const int tabCount, void* data) {
    int activeTab = 0;
    int running = 1;
    
    while (running) {
        system("cls");  // Clear screen
        
        // ═══════════════════════════════════════════════════════
        // DRAW TAB HEADERS
        // ═══════════════════════════════════════════════════════
        for (int i = 0; i < tabCount; i++) {
            if (i == activeTab) {
                printf("[%s]", tabs[i].title);  // Selected tab
            } else {
                printf(" %s  ", tabs[i].title); // Unselected tab
            }
        }
        
        printf("\n===================================================\n");
        
        // ═══════════════════════════════════════════════════════
        // RENDER ACTIVE TAB CONTENT
        // ═══════════════════════════════════════════════════════
        if (tabs[activeTab].renderContent) {
            tabs[activeTab].renderContent(data);
        }
        
        printf("===================================================\n");
        printf("Use arrow keys <- -> to switch tabs. Press ESC to continue.\n");
        
        // ═══════════════════════════════════════════════════════
        // HANDLE INPUT
        // ═══════════════════════════════════════════════════════
        int key = _getch();
        
        if (key == 224) {  // Arrow key prefix
            key = _getch();
            
            if (key == 75) {  // Left arrow
                activeTab--;
                if (activeTab < 0) activeTab = tabCount - 1;
            } else if (key == 77) {  // Right arrow
                activeTab++;
                if (activeTab >= tabCount) activeTab = 0;
            }
        } else if (key == 27) {  // ESC
            running = 0;
        }
    }
}
```

### Usage Example

```c
// Define render functions
void ShowStatsTab(void* data) {
    Character* character = (Character*)data;
    printf("HP: %lld\n", character->attribute.currentHP);
    printf("Attack: %lld\n", character->attribute.current.attack);
    // ... more stats
}

void ShowBlessingsTab(void* data) {
    Character* character = (Character*)data;
    for (int i = 0; i < character->blessingCount; i++) {
        printf("%s (x%lld)\n", 
               character->currentBlessing[i].name,
               character->currentBlessing[i].stacks);
    }
}

// Create tabs
Tab tabs[2] = {
    {"Stats", ShowStatsTab},
    {"Blessings", ShowBlessingsTab}
};

// Show panel
Character player = /*...*/;
showTabPanel(tabs, 2, &player);
```

### Panel Visualization

```
[Attribute Stat]  Blessing Stat  
===================================================
HP                : 450 / 600
Attack            : 125 (base: 100, bonus: 25)
Defense           : 50 (base: 40, bonus: 10)
Crit Chance       : 35%
Crit Damage       : 180%
...
===================================================
Use arrow keys <- -> to switch tabs. Press ESC to continue.
```

When user presses RIGHT ARROW:

```
 Attribute Stat   [Blessing Stat]
===================================================
[1] Divine Wrath
    Rarity        : Legendary
    Stacks        : 5

    Effects       : 4
        Type          : Damage Boost
        Base Value    : 35.00%
        Total Value   : 175.00%
...
===================================================
Use arrow keys <- -> to switch tabs. Press ESC to continue.
```

## 💾 Save System

### Save Data Structure

```c
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
    char blessingNames[20][64];      // Up to 20 blessing names
    long long blessingStacks[20];    // Stack count for each
} RunData;
```

### Save Format

The save system uses a human-readable text format:

```
=== RUN START ===
PlayerName: Adventurer
FloorReached: 25
Timestamp: 1703001234
FinalHP: 450
MaxHP: 800
Attack: 250
Defense: 120
CritChance: 45
CritDamage: 220
DamageBoost: 85
Accuracy: 90
LifeSteal: 15
Regen: 8
BlessingCount: 12
Blessing: Divine Wrath | Stacks: 5 | Rarity: 3
Blessing: Phoenix Blessing | Stacks: 8 | Rarity: 2
Blessing: Minor Strength | Stacks: 20 | Rarity: 0
...
=== RUN END ===
```

### Core Functions

#### Save Run

```c
void SaveRun(const Character* player, const int floorReached) {
    FILE* file = fopen(SAVE_FILE, "a");  // Append mode
    if (file == NULL) {
        printf("Error: Could not open save file!\n");
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
    // ... more stats
    
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
```

#### Load Runs

```c
static int LoadRunsFromFile(RunData runs[], const int maxRuns) {
    FILE* file = fopen(SAVE_FILE, "r");
    if (file == NULL) {
        return 0;  // No save file
    }
    
    int runCount = 0;
    char line[256];
    int inRun = 0;
    RunData currentRun = {0};
    
    while (fgets(line, sizeof(line), file) != NULL && 
           runCount < maxRuns) {
        
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Check for run boundaries
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
            // Parse key-value pairs
            char key[64], value[256];
            if (sscanf(line, "%63[^:]: %255[^\n]", key, value) == 2) {
                // Parse based on key
                if (strcmp(key, "PlayerName") == 0) {
                    strncpy(currentRun.playerName, value, 
                           sizeof(currentRun.playerName) - 1);
                }
                else if (strcmp(key, "FloorReached") == 0) {
                    currentRun.floorReached = atoi(value);
                }
                // ... parse other fields
            }
        }
    }
    
    fclose(file);
    return runCount;
}
```

#### Display Runs

```c
void LoadAndDisplayRuns() {
    RunData runs[MAX_RUNS_DISPLAY];
    const int runCount = LoadRunsFromFile(runs, MAX_RUNS_DISPLAY);
    
    if (runCount == 0) {
        printColor(COL_YELLOW, "No previous runs found!\n");
        return;
    }
    
    int selectedRun = runCount - 1;  // Start with most recent
    int viewing = 1;
    
    while (viewing) {
        system("cls");
        
        // Draw header
        printColor(COL_BOLD, "Previous Runs - Total: %d\n\n", runCount);
        
        // Display list (most recent first)
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
            strftime(timeStr, sizeof(timeStr), 
                    "%Y-%m-%d %H:%M:%S", timeinfo);
            
            printf("[#%d] %s - Floor %d - %s\n",
                   i + 1, runs[i].playerName, 
                   runs[i].floorReached, timeStr);
        }
        
        // Display selected run details
        printf("\n");
        printColor(COL_BOLD, "Selected Run Details:\n");
        RunData* run = &runs[selectedRun];
        
        printColor(COL_CYAN, "Player: ");
        printf("%s\n", run->playerName);
        printColor(COL_YELLOW, "Floor: ");
        printf("%d\n\n", run->floorReached);
        
        printf("Final Stats:\n");
        printf("  HP:          %lld / %lld\n", run->finalHP, run->maxHP);
        printf("  Attack:      %lld\n", run->attack);
        printf("  Defense:     %lld\n", run->defense);
        // ... more stats
        
        if (run->totalBlessings > 0) {
            printf("\nBlessings (%d):\n", run->totalBlessings);
            for (int i = 0; i < 20 && strlen(run->blessingNames[i]) > 0; i++) {
                printf("  - %s", run->blessingNames[i]);
                if (run->blessingStacks[i] > 1) {
                    printColor(COL_GREEN, " x%lld\n", 
                              run->blessingStacks[i]);
                } else {
                    printf("\n");
                }
            }
        }
        
        // Handle input
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {  // Up
                selectedRun++;
                if (selectedRun >= runCount) selectedRun = runCount - 1;
            }
            else if (key == 80) {  // Down
                selectedRun--;
                if (selectedRun < 0) selectedRun = 0;
            }
        }
        else if (key == 27) {  // ESC
            viewing = 0;
        }
    }
}
```

### Get Total Run Count

```c
int GetTotalRunCount() {
    RunData runs[MAX_RUNS_DISPLAY];
    return LoadRunsFromFile(runs, MAX_RUNS_DISPLAY);
}
```

## 🔧 Platform-Specific Considerations

### Windows-Specific Code

```c
#include <conio.h>      // For _getch()
#include <windows.h>    // For console functions

// In main.c
SetConsoleOutputCP(CP_UTF8);  // Enable UTF-8 output
SetConsoleCP(CP_UTF8);        // Enable UTF-8 input
```

### Cross-Platform Alternatives

For Linux/Mac compatibility, consider:

```c
#ifdef _WIN32
    #include <conio.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <ncurses.h>
    #define CLEAR_SCREEN "clear"
    
    // Implement _getch() equivalent using ncurses
    int _getch() {
        return getch();
    }
#endif
```

## 🎨 UI Design Patterns

### Box Drawing

```c
void DrawBox(const char* title) {
    printColor(COL_BOLD, "╔════════════════════════════════════════════╗\n");
    printColor(COL_BOLD, "║ ");
    printColor(COL_CYAN, "%-42s", title);
    printColor(COL_BOLD, " ║\n");
    printColor(COL_BOLD, "╚════════════════════════════════════════════╝\n");
}
```

### Progress Bars

```c
void DrawProgressBar(int current, int max, int width) {
    int filled = (int)((float)current / (float)max * (float)width);
    
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printColor(COL_GREEN, "█");
        } else {
            printColor(COL_BRIGHT_BLACK, "░");
        }
    }
    printf("]");
}
```

### Menu Selection

```c
typedef struct {
    char text[64];
    void (*action)();
} MenuItem;

int ShowMenu(MenuItem items[], int count) {
    int selected = 0;
    
    while (1) {
        system("cls");
        
        for (int i = 0; i < count; i++) {
            if (i == selected) {
                printColor(COL_CYAN, ">>> ");
            } else {
                printf("    ");
            }
            printf("%s\n", items[i].text);
        }
        
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {  // Up
                selected--;
                if (selected < 0) selected = count - 1;
            } else if (key == 80) {  // Down
                selected++;
                if (selected >= count) selected = 0;
            }
        } else if (key == 13) {  // Enter
            return selected;
        }
    }
}
```

## 🐛 Common Issues

### Issue: Colors Not Showing

**Problem**: ANSI codes display as raw text.

**Solution**: Enable UTF-8 and ANSI support:
```c
// Windows 10+
SetConsoleOutputCP(CP_UTF8);
SetConsoleMode(hConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
```

### Issue: Arrow Keys Not Working

**Problem**: `_getch()` returns wrong values.

**Solution**: Arrow keys use two-byte sequences:
```c
int key = _getch();
if (key == 0 || key == 224) {  // Special key prefix
    key = _getch();  // Get actual key code
    // Now check for 72 (up), 80 (down), 75 (left), 77 (right)
}
```

### Issue: Save File Corruption

**Problem**: Save file becomes unreadable.

**Solution**: Validate data before writing:
```c
if (player == NULL || player->name[0] == '\0') {
    return;  // Don't save invalid data
}
```

### Issue: Screen Flickering

**Problem**: Constant redrawing causes flicker.

**Solution**: Only redraw when necessary, use double buffering if needed:
```c
// Don't do this in a tight loop
while (1) {
    system("cls");  // Causes flicker
    DrawUI();
}

// Instead, only clear when needed
if (needsRedraw) {
    system("cls");
    DrawUI();
    needsRedraw = 0;
}
```

## 🚀 Advanced Features

### Custom Color Schemes

```c
typedef struct {
    Color text;
    Color highlight;
    Color success;
    Color warning;
    Color error;
} ColorScheme;

ColorScheme darkTheme = {
    .text = COL_WHITE,
    .highlight = COL_CYAN,
    .success = COL_GREEN,
    .warning = COL_YELLOW,
    .error = COL_RED
};

ColorScheme lightTheme = {
    .text = COL_BLACK,
    .highlight = COL_BLUE,
    .success = COL_GREEN,
    .warning = COL_YELLOW,
    .error = COL_RED
};
```

### Animation System

```c
void AnimateText(const char* text, int delayMs) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        Sleep(delayMs);
    }
    printf("\n");
}

void PulseText(const char* text, int times) {
    for (int i = 0; i < times; i++) {
        printColor(COL_BOLD, "%s", text);
        fflush(stdout);
        Sleep(200);
        
        printf("\r");
        printf("%*s", (int)strlen(text), "");  // Clear
        printf("\r");
        fflush(stdout);
        Sleep(200);
    }
}
```

### Logging System

```c
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void Log(LogLevel level, const char* format, ...) {
    va_list args;
    
    // Timestamp
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    printf("[%02d:%02d:%02d] ", 
           timeinfo->tm_hour, 
           timeinfo->tm_min, 
           timeinfo->tm_sec);
    
    // Level
    switch (level) {
        case LOG_DEBUG:   printColor(COL_BRIGHT_BLACK, "[DEBUG] "); break;
        case LOG_INFO:    printColor(COL_CYAN, "[INFO] "); break;
        case LOG_WARNING: printColor(COL_YELLOW, "[WARN] "); break;
        case LOG_ERROR:   printColor(COL_RED, "[ERROR] "); break;
    }
    
    // Message
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}
```

## 📊 Save Data Statistics

### Save File Analysis

```c
typedef struct {
    int totalRuns;
    int totalFloors;
    int highestFloor;
    char bestPlayer[100];
    float averageFloor;
} SaveStats;

SaveStats AnalyzeSaveFile() {
    SaveStats stats = {0};
    RunData runs[MAX_RUNS_DISPLAY];
    int count = LoadRunsFromFile(runs, MAX_RUNS_DISPLAY);
    
    stats.totalRuns = count;
    
    for (int i = 0; i < count; i++) {
        stats.totalFloors += runs[i].floorReached;
        
        if (runs[i].floorReached > stats.highestFloor) {
            stats.highestFloor = runs[i].floorReached;
            strcpy(stats.bestPlayer, runs[i].playerName);
        }
    }
    
    if (count > 0) {
        stats.averageFloor = (float)stats.totalFloors / (float)count;
    }
    
    return stats;
}
```

## 🔮 Future Enhancements

1. **Binary Save Format**: Faster loading, smaller files
2. **Compression**: Reduce save file size
3. **Cloud Saves**: Sync across devices
4. **Replay System**: Save combat logs for replay
5. **Leaderboards**: Compare with other players
6. **Themes**: Multiple color schemes
7. **Accessibility**: High contrast mode, larger text
8. **Localization**: Multi-language support
9. **Sound**: Add sound effect triggers
10. **Rich Text**: Bold, italic, underline combinations

## 🎯 Best Practices

1. **Always reset color** after colored output
2. **Validate input** before accepting
3. **Check file operations** for errors
4. **Use clear naming** for colors and functions
5. **Provide visual feedback** for all actions
6. **Save frequently** to prevent data loss
7. **Format timestamps** consistently
8. **Cap save file size** to prevent bloat
9. **Test on target platform** before release
10. **Document color meanings** for consistency

---

**Last Updated**: December 2025