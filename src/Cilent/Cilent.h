#pragma once

#include <flecs.h>
#include <Sprender/Sprender.h>

typedef struct Cilent
{
    ecs_world_t* world;
    Sprender* sprender;
} Cilent;

char* Cilent_HelloWorld();

Cilent* Cilent_Create();

void Cilent_Destroy(Cilent* cilent);
