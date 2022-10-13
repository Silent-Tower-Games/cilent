#pragma once

#include <flecs.h>
#include <Sprender/Sprender.h>
#include <Cilent/Config/Config.h>
#include <Cilent/Fonts/FontStash.h>
#include <soloud_c.h>
#include <FPSLoop.h>

typedef struct Cilent {
    ecs_world_t* world;
    Sprender* sprender;
    Cilent_Config config;
    FPSLoop* loop;
    Soloud* soloud;
    char focus;
    FONScontext* fons;
} Cilent;

char* Cilent_HelloWorld();

Cilent* Cilent_Create(
    Cilent_Config config,
    int (*frame)()
);

void Cilent_Loop(Cilent* cilent);

void Cilent_Event_Focus(Cilent* cilent);

void Cilent_Event_Blur(Cilent* cilent);

void Cilent_Destroy(Cilent* cilent);
