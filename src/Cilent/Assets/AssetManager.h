#pragma once

#include <stddef.h>
#include <flecs.h>
#include <Sprender/Texture.h>
#include <vendor/ini-master/src/ini.h>

typedef struct Cilent_AssetManager
{
    size_t textureCount;
    Sprender_Texture* textures;
    ecs_map_t* textureMap;
} Cilent_AssetManager;

Cilent_AssetManager* Cilent_AssetManager_Create();

void Cilent_AssetManager_Load(
    Cilent_AssetManager* assetManager,
    ini_t* modData,
    const char* modName,
    const char* assetType,
    void (*callable)(void*)
);

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    Sprender_Texture* texture
);

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager);
