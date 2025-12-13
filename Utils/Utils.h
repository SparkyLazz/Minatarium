#ifndef UTILS_H
#define UTILS_H

//=====================================
//  PANEL SECTION
//=====================================
typedef struct {
    char title[64];
    void (*renderContent)(void* data);
} Tab;
void showTabPanel(Tab tabs[], int tabCount, void* data);

//=====================================
//  PRINT COLORED
//=====================================
typedef enum {

    // Normal FG
    COL_RESET,
    COL_BLACK,
    COL_RED,
    COL_GREEN,
    COL_YELLOW,
    COL_BLUE,
    COL_MAGENTA,
    COL_CYAN,
    COL_WHITE,

    // Bright FG
    COL_BRIGHT_BLACK,
    COL_BRIGHT_RED,
    COL_BRIGHT_GREEN,
    COL_BRIGHT_YELLOW,
    COL_BRIGHT_BLUE,
    COL_BRIGHT_MAGENTA,
    COL_BRIGHT_CYAN,
    COL_BRIGHT_WHITE,

    // Background normal
    COL_BG_BLACK,
    COL_BG_RED,
    COL_BG_GREEN,
    COL_BG_YELLOW,
    COL_BG_BLUE,
    COL_BG_MAGENTA,
    COL_BG_CYAN,
    COL_BG_WHITE,

    // Background bright
    COL_BG_BRIGHT_BLACK,
    COL_BG_BRIGHT_RED,
    COL_BG_BRIGHT_GREEN,
    COL_BG_BRIGHT_YELLOW,
    COL_BG_BRIGHT_BLUE,
    COL_BG_BRIGHT_MAGENTA,
    COL_BG_BRIGHT_CYAN,
    COL_BG_BRIGHT_WHITE,

    // Styles
    COL_BOLD,
    COL_UNDERLINE,

    // Count
    COL_MAX_ENUM_VALUE

} Color;

void printColor(Color color, const char* format, ...);
#endif