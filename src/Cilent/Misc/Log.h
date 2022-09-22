#pragma once

typedef enum Cilent_Log_Type
{
    CILENT_LOG_ERROR,
    CILENT_LOG_SUCCESS,
    CILENT_LOG_INFO,
} Cilent_Log_Type;

void Cilent_Log(Cilent_Log_Type type, char* fmt, ...);

void Cilent_Log_ToFile(char value);

#define debug_log_type(type, ...) Cilent_Log(CILENT_LOG_##type, __VA_ARGS__)
#define debug_log(...) debug_log_type(INFO, __VA_ARGS__)
