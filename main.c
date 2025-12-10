#include <stdio.h>
#include <stdlib.h>
#include "Entity/Player.h"

void EnterDungeon()
{
    Player player = CreateDefaultPlayer();

    printf("\n");
    printf("============================================================\n");
    printf("              WELCOME TO THE DUNGEON TRAVELER               \n");
    printf("============================================================\n");
    printf(" \"May your journey make a big step further\"\n");
    printf(" Enter your name : ");
    scanf("%99s", player.Name);

    printf("\n");
    printf("====================== PLAYER STATUS =======================\n");
    printf(" Name       : %-20s\n", player.Name);
    printf(" HP         : %-20lld\n", player.attributes.hp);
    printf(" Attack     : %-20lld\n", player.attributes.attack);
    printf(" Defense    : %-20lld\n", player.attributes.defense);
    printf("============================================================\n");
    printf(" Preparing your adventure...\n\n");

    while (player.attributes.hp > 0) {
        int round = 1;
    }
}

void printMenu() {
    printf("\n");
    printf("============================================================\n");
    printf("                       MAIN   MENU                          \n");
    printf("============================================================\n");
    printf(" [ 1 ]  Enter Dungeon\n");
    printf(" [ 0 ]  Exit Game\n");
    printf("------------------------------------------------------------\n");
    printf(" Choose an option: ");
}

int main() {
    // Logo
    printf("$$\\      $$\\ $$\\                      $$\\                         $$\\                         \n");
    printf("$$$\\    $$$ |\\__|                     $$ |                        \\__|                        \n");
    printf("$$$$\\  $$$$ |$$\\ $$$$$$$\\   $$$$$$\\\\ $$$$$$\\    $$$$$$\\   $$$$$$\\  $$\\ $$\\   $$\\ $$$$$$\\\\$$$$\\  \n");
    printf("$$\\$$\\$$ $$ |$$ |$$  __$$\\  \\____$$\\\\_$$  _|   \\____$$\\ $$  __$$\\ $$ |$$ |  $$ |$$  _$$  _$$\\ \n");
    printf("$$ \\$$$  $$ |$$ |$$ |  $$ | $$$$$$$ | $$ |     $$$$$$$ |$$ |  \\__|$$ |$$ |  $$ |$$ / $$ / $$ |\n");
    printf("$$ |\\$  /$$ |$$ |$$ |  $$ |$$  __$$ | $$ |$$\\ $$  __$$ |$$ |      $$ |$$ |  $$ |$$ | $$ | $$ |\n");
    printf("$$ | \\_/ $$ |$$ |$$ |  $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$$ |$$ |      $$ |\\$$$$$$  |$$ | $$ | $$ |\n");
    printf("\\__|     \\__|\\__|\\__|  \\__| \\_______|  \\____/  \\_______|\\__|      \\__| \\______/ \\__| \\__| \\__|\n\n");

    int choice;
    while (1)
    {
        printMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                system("cls"); // clear screen for Windows
                EnterDungeon();
                break;

            case 0:
                printf("\nExiting game... Goodbye!\n");
                return 0;

            default:
                printf("Invalid Choice, try again.\n");
        }
    }
}
