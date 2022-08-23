#include "Log.h"
#include <stdarg.h>
#include <stdio.h>

#define CILENT_LOG_TYPE_TO_STRING_LENGTH 3

static char* Cilent_Log_Type_To_String[CILENT_LOG_TYPE_TO_STRING_LENGTH] = {
    "ERROR",
    "SUCCESS",
    "INFO"
};

void Cilent_Log(Cilent_Log_Type type, char* fmt, ...)
{
    char* typeString = "---";
    
    if (type >= 0 && type < CILENT_LOG_TYPE_TO_STRING_LENGTH) {
        typeString = Cilent_Log_Type_To_String[type];
    }
    
    va_list args;
    va_start(args, fmt);
    printf("LOG [%s]: ", typeString);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}
