#include "Log.h"
#include <stdarg.h>
#include <stdio.h>

static char* Cilent_Log_Type_To_String[3] = {
    "ERROR",
    "SUCCESS"
};

void Cilent_Log(Cilent_Log_Type type, char* fmt, ...)
{
    char* typeString = "---";
    
    if (type >= 0 && type < 2) {
        typeString = Cilent_Log_Type_To_String[type];
    }
    
    // TODO: use type
    va_list args;
    va_start(args, fmt);
    printf("LOG [%s]: ", typeString);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}
