#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ModState.h"
#include "../Flecs/Maps.h"

Cilent_ModState Cilent_ModState_Load(char* activeGame)
{
    Cilent_ModState modState;
    memset(&modState, 0, sizeof(Cilent_ModState));
    
    modState.map = Cilent_Mod_FindAll(
        &modState.games,
        &modState.gamesCount,
        &modState.addons,
        &modState.addonsCount
    );
    
    modState.activeGame = map_get(modState.map, activeGame, Cilent_Mod);
    
    if (modState.activeGame == NULL || !modState.activeGame->game) {
        // TODO: Log that the selected game could not be loaded
        
        modState.activeGame = map_get(modState.map, "base", Cilent_Mod);
        
        assert(modState.activeGame != NULL);
    }
    
    // TODO: Active addons
    modState.activeAddons = malloc(sizeof(Cilent_Mod*) * modState.addonsCount);
    modState.activeAddonsCount = 0;
    
    snprintf(
        activeGame,
        127,
        "%s",
        modState.activeGame->name
    );
    
    return modState;
}

void Cilent_ModState_Destroy(Cilent_ModState modState)
{
    ecs_map_free(modState.map);
    
    for (int i = 0; i < fmax(modState.addonsCount, modState.gamesCount); i++) {
        if (i < modState.gamesCount) {
            Cilent_Mod_Destroy(&modState.games[i]);
        }
        
        if (i < modState.addonsCount) {
            Cilent_Mod_Destroy(&modState.addons[i]);
        }
    }
}
