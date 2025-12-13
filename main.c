#include <stdio.h>
#include "Utils/utils.h"

int main() {
    printColor(COL_MAGENTA, "$$\\      $$\\ $$\\                      $$\\                         $$\\                         \n");
    printColor(COL_MAGENTA, "$$$\\    $$$ |\\__|                     $$ |                        \\__|                        \n");
    printColor(COL_MAGENTA, "$$$$\\  $$$$ |$$\\ $$$$$$$\\   $$$$$$\\\\ $$$$$$\\    $$$$$$\\   $$$$$$\\  $$\\ $$\\   $$\\ $$$$$$\\\\$$$$\\  \n");
    printColor(COL_CYAN,   "$$\\$$\\$$ $$ |$$ |$$  __$$\\  \\____$$\\\\_$$  _|   \\____$$\\ $$  __$$\\ $$ |$$ |  $$ |$$  _$$  _$$\\ \n");
    printColor(COL_CYAN,   "$$ \\$$$  $$ |$$ |$$ |  $$ | $$$$$$$ | $$ |     $$$$$$$ |$$ |  \\__|$$ |$$ |  $$ |$$ / $$ / $$ |\n");
    printColor(COL_BLUE,   "$$ |\\$  /$$ |$$ |$$ |  $$ |$$  __$$ | $$ |$$\\ $$  __$$ |$$ |      $$ |$$ |  $$ |$$ | $$ | $$ |\n");
    printColor(COL_BLUE,   "$$ | \\_/ $$ |$$ |$$ |  $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$$ |$$ |      $$ |\\$$$$$$  |$$ | $$ | $$ |\n");
    printColor(COL_WHITE,  "\\__|     \\__|\\__|\\__|  \\__| \\_______|  \\____/  \\_______|\\__|      \\__| \\______/ \\__| \\__| \\__|\n\n");
    printColor(COL_BRIGHT_BLUE,  "[ 1 ] Start Game\n");
    printColor(COL_BRIGHT_CYAN,  "[ 2 ] Encyclopedia\n");
    printColor(COL_BRIGHT_RED,   "[ 3 ] Quit\n");
    printColor(COL_WHITE, "Choose : ");
    return 0;
}
