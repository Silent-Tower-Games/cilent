#define _DEFAULT_SOURCE
#include "vendor/minunit/minunit.h"
#include <Cilent/Cilent.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Mods/ModState.h>
#include <SDL2/SDL.h>
#include <Sprender/Sprender.h>
#include <assert.h>
#include <flecs.h>
#include <stdio.h>

char* test_one_is_one()
{
    mu_assert(
        "1 should be 1",
        1 == 1);
}

char* UnitTests()
{
    int tests_run = 0;

    mu_run_test(test_one_is_one);

    return NULL;
}

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        char* message = UnitTests();

        if (message != NULL) {
            printf("FAILURE: %s\n", message);
        } else {
            printf("All Tests Passed\n");
        }

        return message != NULL;
    }

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
