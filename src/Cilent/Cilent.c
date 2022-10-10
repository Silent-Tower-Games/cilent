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
    Sprender* sprender = Sprender_Create(
        "Test",
        (Sprender_Int2D) { 640, 360 },
        (Sprender_Int2D) { 320, 180 },
        "data/SpriteEffect.fxb",
        NULL,
        // TODO: make vsync a config setting
        1,
        0
    );
    
    cilent = malloc(sizeof(Cilent));
    cilent->sprender = sprender;
    cilent->world = NULL;
    
    cilent->config = Cilent_Config_Create(config);
    
    cilent->loop = FPSLoop_Create(
        // TODO: make loop type a config setting
        FPSLOOP_TYPE_NOTHING,
        // Always 60?
        60,
        // TODO: create loop callable
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

void Cilent_Destroy(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    
    Sprender_Destroy(cilent->sprender);
    
    free(cilent);
}
