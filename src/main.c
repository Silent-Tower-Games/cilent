#define _DEFAULT_SOURCE
#include <stdio.h>
#include <flecs.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>
#include <Cilent/Mod.h>
#include <Sprender/Sprender.h>

uint64_t Cilent_Hash_String(char* str)
{
    uint64_t hash = 0;
    
    for(int i = 0; str[i] != '\0'; i++) {
        // This is how Java hashes strings
        hash += 31 * hash + str[i];
    }
    
    return hash;
}

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    int numbers[5] = {
        1,
        2,
        3,
        4,
        5,
    };
    
    ecs_map_t* map = ecs_map_new(int, 5);
    ecs_map_set(map, Cilent_Hash_String("one"), &numbers[0]);
    ecs_map_set(map, Cilent_Hash_String("two"), &numbers[1]);
    ecs_map_set(map, Cilent_Hash_String("three"), &numbers[2]);
    ecs_map_set(map, Cilent_Hash_String("four"), &numbers[3]);
    ecs_map_set(map, Cilent_Hash_String("five"), &numbers[4]);
    
    printf("%d\n", ecs_map_get(map, int, Cilent_Hash_String("three")));
    
    return 0;
    
    Cilent_Mod* modsGame = NULL;
    Cilent_Mod* modsAddon = NULL;
    int modsGameCount;
    int modsAddonCount;
    Cilent_Mod_FindAll(&modsGame, &modsGameCount, &modsAddon, &modsAddonCount);
    
    // TODO: Create map of mods
    
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
