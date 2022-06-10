#pragma once

#include "../../vendor/ini-master/src/ini.h"
#include <flecs.h>

typedef struct Cilent_Mod {
    char* iniFilename;
    ini_t* ini;
    char name[128];
    char active;
    char game;
} Cilent_Mod;

ecs_map_t* Cilent_Mod_FindAll(Cilent_Mod** modsGame, int* pModsGameCount, Cilent_Mod** modsMod, int* pModsModCount);

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path);

void Cilent_Mod_Destroy(Cilent_Mod* mod);
