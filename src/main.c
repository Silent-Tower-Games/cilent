#define _DEFAULT_SOURCE
#include <stdio.h>
#include <flecs.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Mods/Mod.h>
#include <Sprender/Sprender.h>

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    ecs_os_init();
    
    Cilent_Mod* modsGame = NULL;
    Cilent_Mod* modsAddon = NULL;
    int modsGameCount;
    int modsAddonCount;
    ecs_map_t* modsMap = Cilent_Mod_FindAll(&modsGame, &modsGameCount, &modsAddon, &modsAddonCount);
    
    printf("%s\n", modsGame[1].name);
    
    Cilent_Mod* modGame = NULL;
    char line[1024];
    char firstRun = 1;
    int scanfSuccess = 1;
    do {
        line[0] = '\0';
        
        if(!scanfSuccess)
        {
            printf("Could not read input\n");
        }
        else if(modGame != NULL)
        {
            printf("Mod `%s` is not a game\n", modGame->name);
        }
        else if(!firstRun)
        {
            printf("Could not find that mod\n");
        }
        
        firstRun = 0;
        
        printf("Which game do you want to play?\n\n");
        
        for(int i = 0; i < modsGameCount; i++)
        {
            char title[1024];
            if(!ini_sget(modsGame[i].ini, "project", "title", "%1023[^\n]", title))
            {
                continue;
            }
            
            printf("- [ %s ] `%s`\n", modsGame[i].name, title);
        }
        
        printf("\n");
        
        scanfSuccess = scanf("%s[^\n]\n", line);
    }
    while(
        !scanfSuccess
        ||
        (modGame = ecs_map_get(modsMap, Cilent_Mod, Cilent_Hash_String(line))) == NULL
        ||
        modGame->game == 0
    );
    
    printf("Now playing `%s`\n", modGame->name);
    
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
    
    printf("Goodbye, World!\n");
    
    return 0;
    
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
}
