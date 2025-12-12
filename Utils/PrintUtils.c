#include <stdio.h>
#include <stdarg.h>
#include "utils.h"

static const char* COLOR_CODES[] = {
    [COL_RESET]    = "\033[0m",
    [COL_RED]      = "\033[31m",
    [COL_GREEN]    = "\033[32m",
    [COL_YELLOW]   = "\033[33m",
    [COL_BLUE]     = "\033[34m",
    [COL_MAGENTA]  = "\033[35m",
    [COL_CYAN]     = "\033[36m",
    [COL_WHITE]    = "\033[37m",
    [COL_BOLD]     = "\033[1m",
    [COL_UNDERLINE]= "\033[4m"
};

void printColor(const Color color, const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Apply color
    printf("%s", COLOR_CODES[color]);

    // Print formatted text
    vprintf(format, args);

    // Reset color
    printf("%s", COLOR_CODES[COL_RESET]);

    va_end(args);
}
