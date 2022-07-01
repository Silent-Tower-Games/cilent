#include "Log.h"
#include <stdarg.h>
#include <stdio.h>

void Cilent_Log(char* fmt, ...)
{
    // TODO: test
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
