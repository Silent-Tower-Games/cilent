#include "Cilent.h"
#include <assert.h>

char* Cilent_HelloWorld()
{
    return "Hello, World!";
}

Cilent* Cilent_Create()
{
    Sprender* sprender = Sprender_Create(
        "Test",
        (Sprender_Int2D) { 640, 360 },
        (Sprender_Int2D) { 320, 180 },
        "data/SpriteEffect.fxb",
        NULL,
        0,
        0);

    Cilent* cilent = malloc(sizeof(Cilent));
    cilent->sprender = sprender;
    cilent->world = NULL;

    return cilent;
}

void Cilent_Destroy(Cilent* cilent)
{
    assert(cilent != NULL);

    Sprender_Destroy(cilent->sprender);

    free(cilent);
}
