#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ModState.h"
#include "../Flecs/Maps.h"
#include "../Misc/Log.h"
#include "../../vendor/ini-master/src/ini.h"

Cilent_ModState Cilent_ModState_Load(char* activeGame, ini_t* configIni, const char* language)
{
    assert(language != NULL);
    assert(strnlen(language, 5) <= 5);
    
    Cilent_ModState modState;
    memset(&modState, 0, sizeof(Cilent_ModState));
    
    modState.map = Cilent_Mod_FindAll(
        &modState.games,
        &modState.gamesCount,
        &modState.addons,
        &modState.addonsCount
    );
    
    modState.activeGame = map_get(modState.map, activeGame, Cilent_Mod);
    
    if (modState.activeGame == NULL || !modState.activeGame->isGame) {
        debug_log("Could not load mod `%s`!", activeGame);
        
        modState.activeGame = map_get(modState.map, "base", Cilent_Mod);
        
        assert(modState.activeGame != NULL);
    }
    
    // TODO: check activeGame for language compliance
    
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
        
        Cilent_ModState_Activate(&modState, modState.addons[i].name, language);
    }
    
    snprintf(
        activeGame,
        127,
        "%s",
        modState.activeGame->name
    );
    
    return modState;
}

void Cilent_ModState_Activate(Cilent_ModState* modState, const char* modKey, const char* language)
{
    assert(modKey != NULL);
    assert(language != NULL);
    assert(strnlen(language, 5) <= 5);
    assert(modState->activeAddonsCount < modState->addonsCount);
    
    Cilent_Mod* mod = map_get(modState->map, modKey, Cilent_Mod);
    
    assert(mod != NULL);
    assert(!mod->active);
    assert(!mod->isGame);
    
    // TODO: check addon for language compliance
    
    mod->active = true;
    
    modState->activeAddons[modState->activeAddonsCount] = mod;
    modState->activeAddonsCount++;
    
    debug_log("Mod is active: `%s`", mod->name);
}

void Cilent_ModState_Deactivate(Cilent_ModState* modState, const char* modKey)
{
    assert(modKey != NULL);
    assert(modState->activeAddonsCount > 0);
    
    Cilent_Mod* mod = map_get(modState->map, modKey, Cilent_Mod);
    
    assert(mod->active);
    assert(!mod->isGame);
    
    char found = -1;
    for (int i = 0; i < modState->activeAddonsCount; i++) {
        if (mod != modState->activeAddons[i]) {
            continue;
        }
        
        found = i;
        
        break;
    }
    
    assert(found != -1);
    assert(found < modState->activeAddonsCount);
    
    mod->active = false;
    memcpy(
        &modState->activeAddons[found],
        &modState->activeAddons[found + 1],
        sizeof(Cilent_Mod*) * (modState->activeAddonsCount - (found + 1))
    );
    modState->activeAddonsCount--;
    
    debug_log("Mod is inactive: `%s`", mod->name);
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
