#pragma once

#include <stddef.h>
#include <flecs.h>
#include <Sprender/Texture.h>
#include <vendor/ini-master/src/ini.h>

typedef struct Cilent_AssetManager_Type
{
    char* list;
    size_t count;
    ecs_map_t* map;
    const char* type;
    size_t size;
} Cilent_AssetManager_Type;

typedef struct Cilent_AssetManager
{
    Cilent_AssetManager_Type shaders;
    Cilent_AssetManager_Type sounds;
    Cilent_AssetManager_Type textures;
} Cilent_AssetManager;

Cilent_AssetManager* Cilent_AssetManager_Create();

void Cilent_AssetManager_Load(
    Cilent_AssetManager_Type* assets,
    ini_t* modData,
    const char* modName,
    void (*callable)(const char*, const char*, void*, ecs_map_t*)
);

void Cilent_AssetManager_Load_Shader(
    const char* filename,
    const char* key,
    void* ptr,
    ecs_map_t* map
);

void Cilent_AssetManager_Load_Sound(
    const char* filename,
    const char* key,
    void* ptr,
    ecs_map_t* map
);

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    const char* key,
    void* ptr,
    ecs_map_t* map
);

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager);
