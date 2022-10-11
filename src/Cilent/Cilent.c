#include "Cilent.h"
#include <Cilent/global.h>
#include <Cilent/Misc/Assert.h>

char* Cilent_HelloWorld()
{
    return "Hello, World!";
}

Cilent* Cilent_Create(
    Cilent_Config config,
    int (*frame)()
)
{
    cilent = malloc(sizeof(Cilent));
    
    ecs_os_init();
    cilent->world = ecs_init();
    
    cilent->soloud = Soloud_create();
    
    // TODO: allow custom window size
    Sprender_Int2D windowSize = { .X = 640, .Y = 360 };
    // TODO: resolution should be set game-by-game
    Sprender_Int2D resolution = { .X = 320, .Y = 180 };
    cilent->sprender = Sprender_Create(
        "Test",
        windowSize,
        resolution,
        "data/SpriteEffect.fxb",
        NULL,
        0,
        0
    );
    
    cilent->config = Cilent_Config_Create(config);
    
    cilent->loop = FPSLoop_Create(
        cilent->config.loopType,
        // Always 60?
        60,
        frame
    );
    
    // FIXME: having to set vsync immediately after creating sprender :(
    // This is because:
    // - sprender creates FNA3D device
    // - Cilent_Config_Create loads all mods
    // - mods load assets
    // - assets need FNA3D device
    // - sprender needs config to know if vsync is on
    // This is easier at the moment than uprooting the application flow lol
    Sprender_SetPresentation(
        cilent->sprender,
        windowSize,
        0,
        cilent->config.vsync
    );
    
    return cilent;
}

void Cilent_Loop(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->loop != NULL);
    
    FPSLoop_Run(cilent->loop);
}

void Cilent_Destroy(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->sprender != NULL);
    CILENT_ASSERT(cilent->soloud != NULL);
    CILENT_ASSERT(cilent->world != NULL);
    
    ecs_fini(cilent->world);
    
    Soloud_destroy(cilent->soloud);
    
    Cilent_Config_Save(&cilent->config);
    Cilent_Config_Destroy(&cilent->config);
    
    Sprender_Destroy(cilent->sprender);
    
    free(cilent);
}
