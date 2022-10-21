#pragma once

#include "Mod.h"
#include <flecs.h>

typedef struct Cilent_ModState {
    Cilent_Mod* games;
    int gamesCount;
    Cilent_Mod* addons;
    int addonsCount;
    Cilent_Mod* activeGame;
    Cilent_Mod** activeAddons;
    int activeAddonsCount;
} Cilent_ModState;

Cilent_ModState Cilent_ModState_Load(char* activeGame, ini_t* configIni, const char* language);

Cilent_Mod* Cilent_ModState_Mod_FindByKey(Cilent_ModState* modState, const char* key);

char Cilent_ModState_Mod_Activate(Cilent_ModState* modState, const char* modKey, const char* language);

void Cilent_ModState_Mod_Deactivate(Cilent_ModState* modState, const char* modKey);

const char* Cilent_ModState_Lang_Find(
    Cilent_ModState* modState,
    char* mod,
    const char* section,
    const char* key
);

void Cilent_ModState_Destroy(Cilent_ModState modState);
