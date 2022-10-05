#pragma once

#include <flecs.h>
#include <Sprender/Sprender.h>
#include <Cilent/Config/Config.h>

typedef struct Cilent {
    ecs_world_t* world;
    Sprender* sprender;
    Cilent_Config config;
} Cilent;

char* Cilent_HelloWorld();

Cilent* Cilent_Create(Cilent_Config config);

void Cilent_Destroy(Cilent* cilent);
