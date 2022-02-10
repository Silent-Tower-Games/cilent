#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "Mod.h"

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path)
{
    assert(path != NULL);
    
    Cilent_Mod mod;
    mod.name = name;
    
    // Build config.ini file path
    char* iniFilename = "config.ini";
    int iniFilenameStrlen = strlen(path) + strlen(iniFilename) + 2;
    
    mod.iniFilename = malloc(sizeof(char) * (iniFilenameStrlen));
    snprintf(mod.iniFilename, iniFilenameStrlen, "%s/%s", path, iniFilename);
    
    // Load config.ini
    FILE* f = fopen(mod.iniFilename, "r");
    assert(f != NULL);
    
    fseek(f, 0, SEEK_END);
    uint32_t codeSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t* code = malloc(codeSize);
    fread(code, 1, codeSize, f);
    fclose(f);
    
    mod.ini = code;
    
    return mod;
}

void Cilent_Mod_Destroy(Cilent_Mod* mod)
{
    assert(mod != NULL);
    
    free(mod->iniFilename);
    free(mod->ini);
}
