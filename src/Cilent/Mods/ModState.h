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

Cilent_ModState Cilent_ModState_Load(char* activeGame);

void Cilent_ModState_Destroy(Cilent_ModState modState);
