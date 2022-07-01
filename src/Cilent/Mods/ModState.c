#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ModState.h"
#include "../Flecs/Maps.h"
#include "../Misc/Log.h"
#include "../../vendor/ini-master/src/ini.h"

Cilent_ModState Cilent_ModState_Load(char* activeGame, ini_t* configIni)
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
        debug_log("Could not load mod `%s`!", activeGame);
        
        modState.activeGame = map_get(modState.map, "base", Cilent_Mod);
        
        assert(modState.activeGame != NULL);
    }
    
    modState.activeAddons = malloc(sizeof(Cilent_Mod*) * modState.addonsCount);
    modState.activeAddonsCount = 0;
    for (int i = 0; i < modState.addonsCount; i++) {
        int isModActive;
        
        if (
            !ini_sget(configIni, "addons", modState.addons[i].name, "%d", &isModActive)
            || !isModActive
        ) {
            continue;
        }
        
        modState.activeAddons[modState.activeAddonsCount] = &modState.addons[i];
        modState.activeAddonsCount++;
        
        debug_log("Mod is active: `%s`", modState.addons[i].name);
    }
    
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
    free(modState.activeAddons);
    
    for (int i = 0; i < fmax(modState.addonsCount, modState.gamesCount); i++) {
        if (i < modState.gamesCount) {
            Cilent_Mod_Destroy(&modState.games[i]);
        }
        
        if (i < modState.addonsCount) {
            Cilent_Mod_Destroy(&modState.addons[i]);
        }
    }
}
