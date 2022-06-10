#pragma once

typedef struct Cilent_Config {
    char app[1024];
    char debug;
    char language[6];
    char org[1024];
    char mod[128];
} Cilent_Config;

Cilent_Config Cilent_Config_Create(Cilent_Config configDefault);

char Cilent_Config_Save(Cilent_Config* config);

char* Cilent_Config_FileData(Cilent_Config* config);
