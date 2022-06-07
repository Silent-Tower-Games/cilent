#define _DEFAULT_SOURCE
#include <Cilent/Cilent.h>
#include <Cilent/Config/Config.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Mods/ModState.h>
#include <SDL2/SDL.h>
#include <Sprender/Sprender.h>
#include <assert.h>
#include <flecs.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    ecs_os_init();
    
    SDL_Init(0);
    
    Cilent_Config config = Cilent_Config_Create((Cilent_Config) {
        .debug = 0,
        .org = "Silent Tower Games",
        .app = "My Game",
        .language = "en",
    });
    printf(Cilent_Config_FileData(&config));
    Cilent_Config_Save(&config);
    
    SDL_Quit();
    
    return 0;
    
    /*
    const char* modName = "my-custom-mod";
    
    Cilent_ModState modState = Cilent_ModState_Load(modName);
    
    assert(modState.activeGame != NULL);
    
    printf("Now playing `%s`\n", modState.activeGame->name);
    
    // Unload mods
    Cilent_ModState_Destroy(modState);
    
    printf("Goodbye, World!\n");
    
    return 0;
    //*/
    
    /*
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
    //*/
}
