#pragma once

#include "../vendor/ini-master/src/ini.h"

typedef struct Cilent_Mod
{
    char* iniFilename;
    ini_t* ini;
    char* name;
    char active;
} Cilent_Mod;

Cilent_Mod* Cilent_Mod_FindAll(int* pModsCount);

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path);

void Cilent_Mod_Destroy(Cilent_Mod* mod);
