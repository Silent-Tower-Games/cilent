#define _DEFAULT_SOURCE
#include <Cilent/Cilent.h>
#include <Cilent/Config/Config.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Misc/Log.h>
#include <Cilent/Mods/ModState.h>
#include <SDL2/SDL.h>
#include <Sprender/Sprender.h>
#include <assert.h>
#include <flecs.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    debug_log("%s", Cilent_HelloWorld());
    
    ecs_os_init();
    
    SDL_Init(0);
    
    Cilent_Config config = Cilent_Config_Create((Cilent_Config) {
        .debug = 0,
        .org = "Silent Tower Games",
        .app = "My Game",
        .language = "en",
        .mod = "my-custom-mod",
    });
    
    assert(config.modState.activeGame != NULL);
    
    debug_log("Now playing `%s`", config.modState.activeGame->name);
    
    debug_log_type(SUCCESS, "This is a success test;");
    debug_log_type(ERROR, "This is an error test.");
    debug_log_type(INFO, "This is an info test!");
    
    // End game
    Cilent_Config_Save(&config);
    Cilent_Config_Destroy(&config);
    
    debug_log("Goodbye, World!");
    
    SDL_Quit();
    
    return 0;
    //*/
    
    /*
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
    //*/
}
