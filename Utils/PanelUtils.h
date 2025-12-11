#ifndef PANEL_UTILS_H
#define PANEL_UTILS_H

#define MAX_TABS 10

typedef struct {
    char title[64];
    void (*renderContent)(void* data);
} Tab;

void showTabPanel(Tab tabs[], const int tabCount, void* data);

#endif
