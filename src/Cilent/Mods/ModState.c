#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ModState.h"
#include <Cilent/Misc/Assert.h>
#include <Cilent/Misc/File.h>
#include <Cilent/Misc/Log.h>
#include <Cilent/vendor/ini-master/src/ini.h>

Cilent_ModState Cilent_ModState_Load(char* activeGame, ini_t* configIni, const char* language)
{
    CILENT_ASSERT(language != NULL);
    CILENT_ASSERT(strnlen(language, 5) <= 5);
    
    Cilent_ModState modState;
    memset(&modState, 0, sizeof(Cilent_ModState));
    
    Cilent_Mod_FindAll(
        &modState.games,
        &modState.gamesCount,
        &modState.addons,
        &modState.addonsCount
    );
    
    modState.activeGame = Cilent_ModState_Mod_FindByKey(&modState, activeGame);
    
    if (modState.activeGame == NULL || !modState.activeGame->isGame) {
        debug_log_type(
            ERROR,
            "Could not load mod `%s`!",
            activeGame
        );
        
        modState.activeGame = Cilent_ModState_Mod_FindByKey(&modState, "base");
    }
    
    CILENT_ASSERT(modState.activeGame != NULL);
    CILENT_ASSERT(Cilent_ModState_Mod_Activate(&modState, modState.activeGame->name, language));
    
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
        
        Cilent_ModState_Mod_Activate(&modState, modState.addons[i].name, language);
    }
    
    snprintf(
        activeGame,
        127,
        "%s",
        modState.activeGame->name
    );
    
    return modState;
}

Cilent_Mod* Cilent_ModState_Mod_FindByKey(Cilent_ModState* modState, const char* key)
{
    CILENT_ASSERT(modState != NULL);
    
    const size_t length = fmax(modState->addonsCount, modState->gamesCount);
    for (int i = 0; i < length; i++)
    {
        // TODO: inefficient! O(n) string comparisons & code repeating
        if (
            i < modState->addonsCount
            && strcmp(key, modState->addons[i].name) == 0
        ) {
            return &modState->addons[i];
        }
        
        if (
            i < modState->gamesCount
            && strcmp(key, modState->games[i].name) == 0
        ) {
            return &modState->games[i];
        }
    }
    
    return NULL;
}

char Cilent_ModState_Mod_Activate(Cilent_ModState* modState, const char* modKey, const char* language)
{
    CILENT_ASSERT(modKey != NULL);
    CILENT_ASSERT(language != NULL);
    CILENT_ASSERT(strnlen(language, 5) <= 5);
    CILENT_ASSERT(modState->activeAddonsCount < modState->addonsCount);
    
    Cilent_Mod* mod = Cilent_ModState_Mod_FindByKey(modState, modKey);
    
    CILENT_ASSERT(mod != NULL);
    CILENT_ASSERT(!mod->active);
    
    if (mod->hasLang)
    {
        mod->lang = Cilent_Lang_Load(modKey, language);
        
        if (mod->lang == NULL) {
            debug_log_type(
                ERROR,
                "Mod not activated: `%s`; missing language file: `%s`",
                mod->name,
                language
            );
            
            return 0;
        }
    }
    
    mod->active = true;
    
    if (!mod->isGame) {
        modState->activeAddons[modState->activeAddonsCount] = mod;
        modState->activeAddonsCount++;
    }
    
    mod->assetManager = Cilent_AssetManager_Create();
    Cilent_AssetManager_Load(
        &mod->assetManager->fonts,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Font
    );
    Cilent_AssetManager_Load(
        &mod->assetManager->scripts,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Script
    );
    Cilent_AssetManager_Load(
        &mod->assetManager->shaders,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Shader
    );
    Cilent_AssetManager_Load(
        &mod->assetManager->sounds,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Sound
    );
    Cilent_AssetManager_Load(
        &mod->assetManager->textures,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Texture
    );
    Cilent_AssetManager_Load(
        &mod->assetManager->tiled,
        mod->ini,
        mod->name,
        Cilent_AssetManager_Load_Tiled
    );
    
    debug_log("Mod is active: `%s`", mod->name);
    
    return 1;
}

void Cilent_ModState_Mod_Deactivate(Cilent_ModState* modState, const char* modKey)
{
    CILENT_ASSERT(modKey != NULL);
    CILENT_ASSERT(modState->activeAddonsCount > 0);
    
    Cilent_Mod* mod = Cilent_ModState_Mod_FindByKey(modState, modKey);
    
    CILENT_ASSERT(mod->active);
    CILENT_ASSERT(!mod->isGame);
    
    char found = -1;
    for (int i = 0; i < modState->activeAddonsCount; i++) {
        if (mod != modState->activeAddons[i]) {
            continue;
        }
        
        found = i;
        
        break;
    }
    
    CILENT_ASSERT(found != -1);
    CILENT_ASSERT(found < modState->activeAddonsCount);
    
    mod->active = false;
    memcpy(
        &modState->activeAddons[found],
        &modState->activeAddons[found + 1],
        sizeof(Cilent_Mod*) * (modState->activeAddonsCount - (found + 1))
    );
    modState->activeAddonsCount--;
    
    debug_log("Mod is inactive: `%s`", mod->name);
}

const char* Cilent_ModState_Lang_Find(
    Cilent_ModState* modState,
    char* mod,
    const char* section,
    const char* key
)
{
    if (
        modState == NULL
        || section == NULL
        || key == NULL
    )
    {
        return NULL;
    }
    
    char* modKey = mod;
    if (modKey == NULL) {
        modKey = modState->activeGame->name;
    }
    
    Cilent_Mod* modObj = Cilent_ModState_Mod_FindByKey(modState, modKey);
    if (
        modObj == NULL
        || modObj->lang == NULL
    )
    {
        return NULL;
    }
    
    return Cilent_Lang_Get(modObj->lang, section, key);
}

void Cilent_ModState_Destroy(Cilent_ModState modState)
{
    free(modState.activeAddons);
    
    for (int i = 0; i < fmax(modState.addonsCount, modState.gamesCount); i++) {
        if (i < modState.gamesCount) {
            Cilent_Mod_Destroy(&modState.games[i]);
        }
        
        if (i < modState.addonsCount) {
            Cilent_Mod_Destroy(&modState.addons[i]);
        }
    }
    
    free(modState.addons);
    free(modState.games);
}
