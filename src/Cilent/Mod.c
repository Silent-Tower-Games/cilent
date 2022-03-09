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

Cilent_Mod* Cilent_Mod_FindAll(int* pModsCount)
{
    struct dirent** list;
    int listLength;
    const char* directory = "data";
    listLength = scandir(directory, &list, NULL, alphasort);
    
    Cilent_Mod* mods = malloc(sizeof(Cilent_Mod) * listLength);
    int modsCount = 0;
    
    const int pathLength = 1024;
    char* path = malloc(sizeof(char) * pathLength);
    while(listLength--)
    {
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
            mods[modsCount] = Cilent_Mod_CreateFromPath(list[listLength]->d_name, path);
            modsCount++;
        }
        
        free(list[listLength]);
    }
    free(path);
    free(list);
    
    *pModsCount = modsCount;
    
    mods = realloc(mods, sizeof(Cilent_Mod) * modsCount);
    
    return mods;
}

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path)
{
    assert(path != NULL);
    
    Cilent_Mod mod;
    mod.name = name;
    mod.active = 0;
    
    printf("Mod: %s\n", mod.name);
    
    // Build config.ini file path
    char* iniFilename = "config.ini";
    int iniFilenameStrlen = strlen(path) + strlen(iniFilename) + 2;
    
    mod.iniFilename = malloc(sizeof(char) * (iniFilenameStrlen));
    snprintf(mod.iniFilename, iniFilenameStrlen, "%s/%s", path, iniFilename);
    
    // Load config.ini
    mod.ini = ini_load(mod.iniFilename);
    
    return mod;
}

void Cilent_Mod_Destroy(Cilent_Mod* mod)
{
    assert(mod != NULL);
    
    free(mod->iniFilename);
    ini_free(mod->ini);
}
