#include <stdio.h>
#include <stdlib.h>
#include "Utils/PanelUtils.h"
#include "Entity/Characters.h"
#include "Game/Combat.h"

void StartGame() {
    Characters player = playerBluePrint;
    printf("==============================================\n");
    printf("Starting with your name : ");
    scanf("%99s", player.name);
    system("cls");

    showPlayerStats(&player);

    // After viewing stats, begin the infinite roguelike run starting at floor 1
    roguelike_start(&player);
}

int main() {
    printf("$$\\      $$\\ $$\\                      $$\\                         $$\\                         \n");
    printf("$$$\\    $$$ |\\__|                     $$ |                        \\__|                        \n");
    printf("$$$$\\  $$$$ |$$\\ $$$$$$$\\   $$$$$$\\\\ $$$$$$\\    $$$$$$\\   $$$$$$\\  $$\\ $$\\   $$\\ $$$$$$\\\\$$$$\\  \n");
    printf("$$\\$$\\$$ $$ |$$ |$$  __$$\\  \\____$$\\\\_$$  _|   \\____$$\\ $$  __$$\\ $$ |$$ |  $$ |$$  _$$  _$$\\ \n");
    printf("$$ \\$$$  $$ |$$ |$$ |  $$ | $$$$$$$ | $$ |     $$$$$$$ |$$ |  \\__|$$ |$$ |  $$ |$$ / $$ / $$ |\n");
    printf("$$ |\\$  /$$ |$$ |$$ |  $$ |$$  __$$ | $$ |$$\\ $$  __$$ |$$ |      $$ |$$ |  $$ |$$ | $$ | $$ |\n");
    printf("$$ | \\_/ $$ |$$ |$$ |  $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$$ |$$ |      $$ |\\$$$$$$  |$$ | $$ | $$ |\n");
    printf("\\__|     \\__|\\__|\\__|  \\__| \\_______|  \\____/  \\_______|\\__|      \\__| \\______/ \\__| \\__| \\__|\n\n");
    printf("[ 1 ] Start Game\n");
    printf("[ 2 ] Encyclopedia\n");
    printf("[ 3 ] Roguelike Demo (Endless Floors)\n");
    printf("[ 4 ] Quit\n");
    printf("Your choice : ");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            system("cls");
            StartGame();
            break;

        case 2:
            system("cls");
            printf("Opening encyclopedia...\n");
            // Call your encyclopedia function here
            break;

        case 3:
            system("cls");
            roguelike_demo();
            break;

        case 4:
            printf("Exiting game. Goodbye!\n");
            return 0; // Exit the program

        default:
            printf("Invalid choice! Press Enter to try again.\n");
            getchar(); // consume newline left by scanf
            getchar(); // wait for user to press Enter
    }
}