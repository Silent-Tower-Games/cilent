#pragma once

#include <stddef.h>
#include <flecs.h>
#include <Sprender/Texture.h>
#include <Cilent/vendor/ini-master/src/ini.h>

typedef struct Cilent_AssetManager_Type
{
    void** list;
    char** names;
    size_t count;
    const char* type;
    size_t size;
} Cilent_AssetManager_Type;

typedef struct Cilent_AssetManager
{
    Cilent_AssetManager_Type fonts;
    Cilent_AssetManager_Type scripts;
    Cilent_AssetManager_Type shaders;
    Cilent_AssetManager_Type sounds;
    Cilent_AssetManager_Type textures;
} Cilent_AssetManager;

Cilent_AssetManager* Cilent_AssetManager_Create();

void Cilent_AssetManager_Load(
    Cilent_AssetManager_Type* assets,
    ini_t* modData,
    const char* modName,
    void (*callable)(const char*, const char*, void*)
);

void Cilent_AssetManager_Load_Font(
    const char* filename,
    const char* key,
    void* ptr
);

void Cilent_AssetManager_Load_Script(
    const char* filename,
    const char* key,
    void* ptr
);

void Cilent_AssetManager_Load_Shader(
    const char* filename,
    const char* key,
    void* ptr
);

void Cilent_AssetManager_Load_Sound(
    const char* filename,
    const char* key,
    void* ptr
);

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    const char* key,
    void* ptr
);

void* Cilent_AssetManagerType_FindByKey(
    Cilent_AssetManager_Type* assetManagerType,
    const char* key
);

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager);
