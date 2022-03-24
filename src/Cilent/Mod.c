#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "Mod.h"

void Cilent_Mod_FindAll(Cilent_Mod** pModsGame, int* pModsGameCount, Cilent_Mod** pModsAddon, int* pModsAddonCount)
{
    // Assert these pointers actually go somewhere
    assert(pModsGame != NULL);
    assert(pModsAddon != NULL);
    // Assert that the pointers' values are null pointers
    // This is to assure that we aren't throwing away any memory by setting them
    assert(*pModsGame == NULL);
    assert(*pModsAddon == NULL);
    
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
    Cilent_Mod* modsMod = malloc(sizeof(Cilent_Mod) * listLength);
    
    // Reset these to 0 before we start counting up
    *pModsGameCount = 0;
    *pModsAddonCount = 0;
    
    // The relative path to a mod should probably not be longer than this
    const int pathLength = 1024;
    char* path = malloc(sizeof(char) * pathLength);
    while(listLength--)
    {
        // Skip . and .. as they are not real directories
        if(
            strcmp(list[listLength]->d_name, ".") == 0
            ||
            strcmp(list[listLength]->d_name, "..") == 0
        )
        {
            continue;
        }
        
        if(snprintf(path, pathLength, "%s/%s", directory, list[listLength]->d_name) >= pathLength)
        {
            printf("Path name too long (limit: %d characters)! `%s/%s`\n", pathLength - 1, directory, list[listLength]->d_name);
            continue;
        }
        
        struct stat stats;
        stat(path, &stats);
        
        if(S_ISDIR(stats.st_mode))
        {
            Cilent_Mod mod = Cilent_Mod_CreateFromPath(list[listLength]->d_name, path);
            printf("Adding a mod: %s\n", mod.name);
            
            if(mod.game)
            {
                modsGame[*pModsGameCount] = mod;
                (*pModsGameCount)++;
            }
            else
            {
                modsMod[*pModsAddonCount] = mod;
                (*pModsAddonCount)++;
            }
        }
        
        free(list[listLength]);
    }
    free(path);
    free(list);
    
    *pModsGame = realloc(modsGame, sizeof(Cilent_Mod) * *pModsGameCount);
    *pModsAddon = realloc(modsMod, sizeof(Cilent_Mod) * *pModsAddonCount);
}

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path)
{
    assert(path != NULL);
    
    Cilent_Mod mod;
    mod.name = name;
    mod.active = 0;
    mod.game = 0;
    
    // Build config.ini file path
    char* iniFilename = "config.ini";
    int iniFilenameStrlen = strlen(path) + strlen(iniFilename) + 2;
    
    mod.iniFilename = malloc(sizeof(char) * (iniFilenameStrlen));
    snprintf(mod.iniFilename, iniFilenameStrlen, "%s/%s", path, iniFilename);
    
    // Load config.ini
    mod.ini = ini_load(mod.iniFilename);
    
    // Set whether or not this is a full game mod
    ini_sget(mod.ini, "project", "game", "%c", &mod.game);
    
    return mod;
}

void Cilent_Mod_Destroy(Cilent_Mod* mod)
{
    assert(mod != NULL);
    
    free(mod->iniFilename);
    ini_free(mod->ini);
}
