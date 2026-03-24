#include "mate/error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error_fatal(char *fmate, ...)
{
        va_list args;
        va_start(args, fmate);
        fprintf(stderr, "[ERROR] ");
        vprintf(fmate, args);
        printf("\n");
        va_end(args);
        exit(1);
}