#include <dirent.h>
#include <flecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Misc/Log.h>
#include "Mod.h"

void Cilent_Mod_Init();

void Cilent_Mod_FindAll(Cilent_Mod** pModsGame, int* pModsGameCount, Cilent_Mod** pModsAddon, int* pModsAddonCount)
{
    // Assert these pointers actually go somewhere
    CILENT_ASSERT(pModsGame != NULL);
    CILENT_ASSERT(pModsAddon != NULL);
    // Assert that the pointers' values are null pointers
    // This is to assure that we aren't throwing away any memory by setting them
    CILENT_ASSERT(*pModsGame == NULL);
    CILENT_ASSERT(*pModsAddon == NULL);
    
    // Set up all of the directory searching stuff
    // This also gets us the highest possible mod count in listLength
    // It's probably always gonna be 2 more than the highest possible, actually
    struct dirent** list;
    int listLength;
    const char* directory = "data";
    listLength = scandir(directory, &list, NULL, alphasort);
    
    // Initialize both of these at the highest possible length
    // We'll realloc later to free any unused memory
    // At least one of these must be smaller than the highest possible length
    Cilent_Mod* modsGame = malloc(sizeof(Cilent_Mod) * listLength);
    Cilent_Mod* modsAddon = malloc(sizeof(Cilent_Mod) * listLength);
    
    // Reset these to 0 before we start counting up
    *pModsGameCount = 0;
    *pModsAddonCount = 0;
    
    // The relative path to a mod should probably not be longer than this
    const int pathLength = 1024;
    char* path = malloc(sizeof(char) * pathLength);
    while (listLength-- > 0)
    {
        // Skip `.` and `..` as they are not real directories
        // Also skip `engine` as it will only have assets
        if (
            strcmp(list[listLength]->d_name, ".") == 0
            || strcmp(list[listLength]->d_name, "..") == 0
            || strcmp(list[listLength]->d_name, "engine") == 0
        ) {
            free(list[listLength]);
            continue;
        }
        
        if (snprintf(path, pathLength, "%s/%s", directory, list[listLength]->d_name) >= pathLength) {
            debug_log_type(
                ERROR,
                "Path name too long (limit: %d characters)! `%s/%s`",
                pathLength - 1,
                directory,
                list[listLength]->d_name
            );
            free(list[listLength]);
            continue;
        }
        
        struct stat stats;
        stat(path, &stats);
        
        if (S_ISDIR(stats.st_mode)) {
            Cilent_Mod mod = Cilent_Mod_CreateFromPath(list[listLength]->d_name, path);
            Cilent_Mod* pMod = NULL;
            
            if (mod.isGame) {
                pMod = &modsGame[*pModsGameCount];
                (*pModsGameCount)++;
            } else {
                pMod = &modsAddon[*pModsAddonCount];
                (*pModsAddonCount)++;
            }
            
            *pMod = mod;
        }
        
        free(list[listLength]);
    }
    free(path);
    free(list);
    
    *pModsGame = realloc(modsGame, sizeof(Cilent_Mod) * (*pModsGameCount));
    *pModsAddon = realloc(modsAddon, sizeof(Cilent_Mod) * (*pModsAddonCount));
}

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path)
{
    CILENT_ASSERT(path != NULL);
    
    Cilent_Mod mod;
    memset(&mod, 0, sizeof(mod));
    snprintf(mod.name, 127, "%s", name);
    
    // Build config.ini file path
    char* iniFilename = "config.ini";
    int iniFilenameStrlen = strlen(path) + strlen(iniFilename) + 2;
    
    mod.iniFilename = malloc(sizeof(char) * (iniFilenameStrlen));
    snprintf(mod.iniFilename, iniFilenameStrlen, "%s/%s", path, iniFilename);
    
    // Load config.ini
    mod.ini = ini_load(mod.iniFilename);
    
    CILENT_ASSERT(mod.ini != NULL);
    
    // Set whether or not this is a full game mod
    mod.isGame = (
        ini_sget(mod.ini, "project", "game", "%d", &mod.isGame)
        && mod.isGame
    );
    
    // Set whether or not this mod needs a language file
    mod.hasLang = (
        ini_sget(mod.ini, "project", "lang", "%d", &mod.hasLang)
        && mod.hasLang
    );
    
    return mod;
}

void Cilent_Mod_Destroy(Cilent_Mod* mod)
{
    CILENT_ASSERT(mod != NULL);
    
    Cilent_AssetManager_Destroy(mod->assetManager);
    
    CILENT_ASSERT(mod->hasLang == (mod->lang != NULL));
    if (mod->lang != NULL) {
        Cilent_Lang_Destroy(mod->lang);
    }
    
    free(mod->iniFilename);
    ini_free(mod->ini);
}
