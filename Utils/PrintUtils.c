#include <stdio.h>
#include <stdarg.h>
#include "utils.h"

static const char* COLOR_CODES[] = {

    // Normal FG
    [COL_RESET]            = "\033[0m",
    [COL_BLACK]            = "\033[30m",
    [COL_RED]              = "\033[31m",
    [COL_GREEN]            = "\033[32m",
    [COL_YELLOW]           = "\033[33m",
    [COL_BLUE]             = "\033[34m",
    [COL_MAGENTA]          = "\033[35m",
    [COL_CYAN]             = "\033[36m",
    [COL_WHITE]            = "\033[37m",

    // Bright FG
    [COL_BRIGHT_BLACK]     = "\033[90m",
    [COL_BRIGHT_RED]       = "\033[91m",
    [COL_BRIGHT_GREEN]     = "\033[92m",
    [COL_BRIGHT_YELLOW]    = "\033[93m",
    [COL_BRIGHT_BLUE]      = "\033[94m",
    [COL_BRIGHT_MAGENTA]   = "\033[95m",
    [COL_BRIGHT_CYAN]      = "\033[96m",
    [COL_BRIGHT_WHITE]     = "\033[97m",

    // Background normal
    [COL_BG_BLACK]         = "\033[40m",
    [COL_BG_RED]           = "\033[41m",
    [COL_BG_GREEN]         = "\033[42m",
    [COL_BG_YELLOW]        = "\033[43m",
    [COL_BG_BLUE]          = "\033[44m",
    [COL_BG_MAGENTA]       = "\033[45m",
    [COL_BG_CYAN]          = "\033[46m",
    [COL_BG_WHITE]         = "\033[47m",

    // Background bright
    [COL_BG_BRIGHT_BLACK]  = "\033[100m",
    [COL_BG_BRIGHT_RED]    = "\033[101m",
    [COL_BG_BRIGHT_GREEN]  = "\033[102m",
    [COL_BG_BRIGHT_YELLOW] = "\033[103m",
    [COL_BG_BRIGHT_BLUE]   = "\033[104m",
    [COL_BG_BRIGHT_MAGENTA]= "\033[105m",
    [COL_BG_BRIGHT_CYAN]   = "\033[106m",
    [COL_BG_BRIGHT_WHITE]  = "\033[107m",

    // Styles
    [COL_BOLD]             = "\033[1m",
    [COL_UNDERLINE]        = "\033[4m"
};

void printColor(const Color color, const char* format, ...) {
    va_list args;

    // Safety: if invalid color index is passed
    Color safeColor = color;
    if (safeColor >= COL_MAX_ENUM_VALUE)
        safeColor = COL_RESET;

    // Apply color
    printf("%s", COLOR_CODES[safeColor]);

    // Print formatted text
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Reset color
    printf("%s", COLOR_CODES[COL_RESET]);
}
