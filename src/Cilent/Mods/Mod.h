#pragma once

#include "../../vendor/ini-master/src/ini.h"
#include <flecs.h>
#include <Cilent/Assets/AssetManager.h>
#include <Cilent/Lang/Lang.h>

typedef struct Cilent_Mod {
    // TODO: maybe we don't need to keep the mod ini filename around lol
    char* iniFilename;
    ini_t* ini;
    char name[128];
    char active;
    char isGame;
    char hasLang;
    Cilent_Lang* lang;
    Cilent_AssetManager* assetManager;
} Cilent_Mod;

void Cilent_Mod_FindAll(Cilent_Mod** modsGame, int* pModsGameCount, Cilent_Mod** modsMod, int* pModsModCount);

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path);

void Cilent_Mod_Destroy(Cilent_Mod* mod);
