#include <stdio.h>
#include <stdlib.h>

#include "Entity/Player.h"

void EnterDungeon()
{
    //Register Player
    Player player = {0};
    printf("================================================\n");
    printf("\"May your journey make a big step further\"\n");
    printf("Your Name : ");
    scanf("%99s", player.Name);

    //Add Value in Player

    printf("==================== Player Stat ==================\n");
    printf("Your Name : %s\n", player.Name);
    printf("HP        : %lld\n", player.attributes.hp);
    printf("Attack    : %lld\n", player.attributes.attack);
    printf("Defense   : %lld\n", player.attributes.defense);
    printf("===================================================\n");
    printf("Starting Dungeon ...\n");

    //Looping Game
    while (player.HP > 0) {

    }




}
void printMenu() {
    printf("=====================================\n");
    printf("              MAIN MENU              \n");
    printf("=====================================\n");
    printf("[ 1 ]  Enter Dungeon\n");
    printf("[ 0 ]  Exit Game\n");
    printf("=====================================\n");
    printf("Choose: ");
}

int main() {
    //Games
    printf("$$\\      $$\\ $$\\                      $$\\                         $$\\                         \n");
    printf("$$$\\    $$$ |\\__|                     $$ |                        \\__|                        \n");
    printf("$$$$\\  $$$$ |$$\\ $$$$$$$\\   $$$$$$\\\\ $$$$$$\\    $$$$$$\\   $$$$$$\\  $$\\ $$\\   $$\\ $$$$$$\\\\$$$$\\  \n");
    printf("$$\\$$\\$$ $$ |$$ |$$  __$$\\  \\____$$\\\\_$$  _|   \\____$$\\ $$  __$$\\ $$ |$$ |  $$ |$$  _$$  _$$\\ \n");
    printf("$$ \\$$$  $$ |$$ |$$ |  $$ | $$$$$$$ | $$ |     $$$$$$$ |$$ |  \\__|$$ |$$ |  $$ |$$ / $$ / $$ |\n");
    printf("$$ |\\$  /$$ |$$ |$$ |  $$ |$$  __$$ | $$ |$$\\ $$  __$$ |$$ |      $$ |$$ |  $$ |$$ | $$ | $$ |\n");
    printf("$$ | \\_/ $$ |$$ |$$ |  $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$$ |$$ |      $$ |\\$$$$$$  |$$ | $$ | $$ |\n");
    printf("\\__|     \\__|\\__|\\__|  \\__| \\_______|  \\____/  \\_______|\\__|      \\__| \\______/ \\__| \\__| \\__|\n");
    printf("\n");

    //Player Choice
    int choice;
    while (1)
    {
        printMenu();
        scanf("%d",&choice);
        switch (choice) {
            case 1:
                EnterDungeon();
                break;
            case 0:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid Choice \n");
        }
    }
}