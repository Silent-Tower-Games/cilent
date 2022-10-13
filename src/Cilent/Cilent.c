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
    
    cilent->focus = 1;
    
    // Flecs
    ecs_os_init();
    cilent->world = ecs_init();
    
    // Soloud
    cilent->soloud = Soloud_create();
    Soloud_init(cilent->soloud);
    
    // Sprender
    // TODO: allow custom window size
    Sprender_Int2D windowSize = { .X = 640, .Y = 360 };
    // TODO: resolution should be set game-by-game
    Sprender_Int2D resolution = { .X = 320, .Y = 180 };
    cilent->sprender = Sprender_Create(
        "Test",
        windowSize,
        resolution,
        "data/engine/SpriteEffect.fxb",
        NULL,
        0,
        0
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
    
    // FontStash
    memset(&cilent->fontStashSprender, 0, sizeof(FontStashSprender));
    FONSparams fonsParams = {
        // TODO: width & height must be adjustable
        // TODO: what about when switching languages? regenerate fontstash?
        .width = 128,
        .height = 128,
        .flags = FONS_ZERO_TOPLEFT,
        .userPtr = &cilent->fontStashSprender,
        .renderCreate = FONS_renderCreate,
        .renderResize = FONS_renderResize,
        .renderUpdate = FONS_renderUpdate,
        .renderDraw = FONS_renderDraw,
        .renderDelete = FONS_renderDelete,
    };
    cilent->fons = fonsCreateInternal(&fonsParams);
    cilent->defaultFont = fonsAddFont(
        cilent->fons,
        "default",
        "data/engine/PressStart2P/PressStart2P.ttf"
    );
    
    // Config
    cilent->config = Cilent_Config_Create(config);
    
    // FPSLoop
    cilent->loop = FPSLoop_Create(
        cilent->config.loopType,
        // Always 60?
        60,
        frame
    );
    
    return cilent;
}

void Cilent_Loop(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->loop != NULL);
    
    FPSLoop_Run(cilent->loop);
}

void Cilent_Event_Focus(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->soloud != NULL);
    
    Soloud_setPauseAll(cilent->soloud, 0);
    
    cilent->focus = 1;
}

void Cilent_Event_Blur(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->soloud != NULL);
    
    Soloud_setPauseAll(cilent->soloud, 1);
    
    cilent->focus = 0;
}

void Cilent_Destroy(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    CILENT_ASSERT(cilent->sprender != NULL);
    CILENT_ASSERT(cilent->soloud != NULL);
    CILENT_ASSERT(cilent->world != NULL);
    CILENT_ASSERT(cilent->fons != NULL);
    
    ecs_fini(cilent->world);
    
    FontStashSprender* fsp = (FontStashSprender*)((FONSparams*)cilent->fons)->userPtr;
    
    fonsDeleteInternal(cilent->fons);
    
    Soloud_deinit(cilent->soloud);
    Soloud_destroy(cilent->soloud);
    
    Cilent_Config_Save(&cilent->config);
    Cilent_Config_Destroy(&cilent->config);
    
    Sprender_Destroy(cilent->sprender);
    
    free(cilent);
}
