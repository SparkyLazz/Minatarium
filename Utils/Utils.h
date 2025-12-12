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
    COL_RESET,
    COL_RED,
    COL_GREEN,
    COL_YELLOW,
    COL_BLUE,
    COL_MAGENTA,
    COL_CYAN,
    COL_WHITE,
    COL_BOLD,
    COL_UNDERLINE
} Color;

void printColor(Color color, const char* format, ...);
#endif