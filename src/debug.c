#include "mate/debug.h"

#include <stdio.h>
#include <stdlib.h>

void printd(const char *fmate, ...)
{
#ifdef DEBUG
        va_list args;
        va_start(args, fmate);
        vprintf(fmate, args);
        va_end(args);
#else
        (void)fmate;
#endif
}
