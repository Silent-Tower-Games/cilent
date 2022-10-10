#pragma once

#include <flecs.h>
#include <Sprender/Sprender.h>
#include <Cilent/Config/Config.h>
#include <FPSLoop.h>

typedef struct Cilent {
    ecs_world_t* world;
    Sprender* sprender;
    Cilent_Config config;
    FPSLoop* loop;
} Cilent;

char* Cilent_HelloWorld();

Cilent* Cilent_Create(
    Cilent_Config config,
    int (*frame)()
);

void Cilent_Loop(Cilent* cilent);

void Cilent_Destroy(Cilent* cilent);
