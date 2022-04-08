#pragma once

#include <Sprender/Sprender.h>
#include <flecs.h>

typedef struct Cilent {
    ecs_world_t* world;
    Sprender* sprender;
} Cilent;

char* Cilent_HelloWorld();

Cilent* Cilent_Create();

void Cilent_Destroy(Cilent* cilent);
