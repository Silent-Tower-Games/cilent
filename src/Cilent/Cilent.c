#include "Cilent.h"
#include <Cilent/global.h>
#include <Cilent/Misc/Assert.h>

char* Cilent_HelloWorld()
{
    return "Hello, World!";
}

Cilent* Cilent_Create(Cilent_Config config)
{
    Sprender* sprender = Sprender_Create(
        "Test",
        (Sprender_Int2D) { 640, 360 },
        (Sprender_Int2D) { 320, 180 },
        "data/SpriteEffect.fxb",
        NULL,
        0,
        0
    );
    
    cilent = malloc(sizeof(Cilent));
    cilent->sprender = sprender;
    cilent->world = NULL;
    
    cilent->config = Cilent_Config_Create(config);

    return cilent;
}

void Cilent_Destroy(Cilent* cilent)
{
    CILENT_ASSERT(cilent != NULL);
    
    Sprender_Destroy(cilent->sprender);
    
    free(cilent);
}
