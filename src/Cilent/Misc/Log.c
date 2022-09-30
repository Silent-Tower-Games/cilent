#include "Log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CILENT_LOG_TYPE_TO_STRING_LENGTH 3

static char* Cilent_Log_Type_To_String[CILENT_LOG_TYPE_TO_STRING_LENGTH] = {
    "ERROR",
    "SUCCESS",
    "INFO"
};

static char Cilent_Log_ToFile_Value = 0;

static const char* Cilent_Log_Timestamp()
{
    const size_t textLength = 21;
    char* text = malloc(sizeof(char) * textLength);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    strftime(text, textLength - 1, "%Y/%m/%d %H:%M:%S", t);
    
    return text;
}

void Cilent_Log(Cilent_Log_Type type, char* fmt, ...)
{
    char* typeString = "---";
    
    if (type >= 0 && type < CILENT_LOG_TYPE_TO_STRING_LENGTH) {
        typeString = Cilent_Log_Type_To_String[type];
    }
    
    va_list args;
    va_start(args, fmt);
    
    if (Cilent_Log_ToFile_Value)
    {
        FILE* file = fopen("log.txt", "a");
        fprintf(file, "%s [%s]: ", Cilent_Log_Timestamp(), typeString);
        vfprintf(file, fmt, args);
        fprintf(file, "\n");
        fclose(file);
    }
    else
    {
        printf("%s [%s]: ", Cilent_Log_Timestamp(), typeString);
        vprintf(fmt, args);
        printf("\n");
    }
    
    va_end(args);
}

void Cilent_Log_ToFile(char value)
{
    Cilent_Log_ToFile_Value = value;
}
