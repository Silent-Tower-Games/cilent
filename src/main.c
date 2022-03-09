#define _DEFAULT_SOURCE
#include <stdio.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>
#include <Cilent/Mod.h>

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    int modsCount;
    Cilent_Mod* mods = Cilent_Mod_FindAll(&modsCount);
    
    // Cycle through mods
    for(int i = 0; i < modsCount; i++)
    {
        printf(
            "\"%s\" by %s\n",
            ini_get(mods[i].ini, "project", "title"),
            ini_get(mods[i].ini, "project", "developer")
        );
        
        int testVersionNumber;
        if(ini_sget(mods[i].ini, "project", "version", "%d", &testVersionNumber))
        {
            printf("Version: %d\n",testVersionNumber);
        }
    }
    
    // Unload mods
    for(int i = 0; i < modsCount; i++)
    {
        Cilent_Mod_Destroy(&mods[i]);
    }
    
    return 0;
    
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
}
