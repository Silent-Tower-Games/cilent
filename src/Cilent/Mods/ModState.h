#pragma once

#include "Mod.h"
#include <flecs.h>

typedef struct Cilent_ModState {
    ecs_map_t* map;
    Cilent_Mod* games;
    int gamesCount;
    Cilent_Mod* addons;
    int addonsCount;
    Cilent_Mod* activeGame;
    Cilent_Mod** activeAddons;
    int activeAddonsCount;
} Cilent_ModState;

Cilent_ModState Cilent_ModState_Load(char* activeGame, ini_t* configIni, const char* language);

char Cilent_ModState_Activate(Cilent_ModState* modState, const char* modKey, const char* language);

void Cilent_ModState_Deactivate(Cilent_ModState* modState, const char* modKey);

void Cilent_ModState_Destroy(Cilent_ModState modState);
