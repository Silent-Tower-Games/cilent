#define _DEFAULT_SOURCE
#include <stdio.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>
#include <Cilent/Mod.h>
#include <Sprender/Sprender.h>

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    Cilent_Mod* modsGame = NULL;
    Cilent_Mod* modsAddon = NULL;
    int modsGameCount;
    int modsAddonCount;
    Cilent_Mod_FindAll(&modsGame, &modsGameCount, &modsAddon, &modsAddonCount);
    
    // Cycle through mods
    for(int i = 0; i < fmax(modsAddonCount, modsGameCount); i++)
    {
        if(i < modsGameCount)
        {
            printf(
                "Game: \"%s\" by %s\n",
                ini_get(modsGame[i].ini, "project", "title"),
                ini_get(modsGame[i].ini, "project", "developer")
            );
        }
        
        if(i < modsAddonCount)
        {
            printf(
                "Addon: \"%s\" by %s\n",
                ini_get(modsGame[i].ini, "project", "title"),
                ini_get(modsGame[i].ini, "project", "developer")
            );
        }
    }
    
    // Unload mods
    for(int i = 0; i < fmax(modsAddonCount, modsGameCount); i++)
    {
        if(i < modsGameCount)
        {
            Cilent_Mod_Destroy(&modsGame[i]);
        }
        
        if(i < modsAddonCount)
        {
            Cilent_Mod_Destroy(&modsAddon[i]);
        }
    }
    
    return 0;
    
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
}
