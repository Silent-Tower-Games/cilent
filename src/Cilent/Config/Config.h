#pragma once

#include "../Mods/ModState.h"

typedef struct Cilent_Config {
    char app[1024];
    char debug;
    char language[6];
    char org[1024];
    char game[128];
    Cilent_ModState modState;
} Cilent_Config;

Cilent_Config Cilent_Config_Create(Cilent_Config configDefault);

char Cilent_Config_Save(Cilent_Config* config);

void Cilent_Config_Destroy(Cilent_Config* config);
