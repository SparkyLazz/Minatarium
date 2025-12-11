#include <stdio.h>
#include <conio.h>
#include <windows.h>
#define MAX_TABS 10

typedef struct {
    char title[64];
    void (*renderContent)(void* data);
}Tab;

void showTabPanel(Tab tabs[], const int tabCount, void* data) {
    int activeTab = 0;
    int running = 1;

    while (running) {
        system("cls");
        for (int i = 0; i < tabCount; i++) {
            if (i == activeTab) {
                printf("[%s]", tabs[i].title);
            }
            else {
                printf(" %s  ", tabs[i].title);
            }
        }
        printf("\n===================================================\n");
        // Render active tab content
        if(tabs[activeTab].renderContent)
            tabs[activeTab].renderContent(data);
        printf("===================================================\n");
        printf("Use arrow keys ← → to switch tabs. Press ESC to exit.\n");

        // Handle input
        int key = _getch();
        if(key == 224) {
            key = _getch();
            if(key == 75) { // left arrow
                activeTab--;
                if(activeTab < 0) activeTab = tabCount - 1;
            } else if(key == 77) { // right arrow
                activeTab++;
                if(activeTab >= tabCount) activeTab = 0;
            }
        } else if(key == 27) { // ESC
            running = 0;
        }
    }
}