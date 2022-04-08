#define _DEFAULT_SOURCE
#include <Cilent/Cilent.h>
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

    const char* modName = "my-custom-mod";

    Cilent_ModState modState = Cilent_ModState_Load(modName);

    assert(modState.activeGame != NULL);

    printf("Now playing `%s`\n", modState.activeGame->name);

    // Unload mods
    Cilent_ModState_Destroy(modState);

    printf("Goodbye, World!\n");

    return 0;

    Cilent* cilent = Cilent_Create();

    SDL_Delay(1000);

    Cilent_Destroy(cilent);

    return 0;
}
