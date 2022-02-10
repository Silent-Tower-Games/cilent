#pragma once

typedef struct Cilent_Mod
{
    char* iniFilename;
    char* ini;
    char* name;
} Cilent_Mod;

Cilent_Mod Cilent_Mod_CreateFromPath(char* name, char* path);

void Cilent_Mod_Destroy(Cilent_Mod* mod);
