#include "AssetManager.h"

#include <dirent.h>
#include <lua.h>
#include <stdio.h>
#include <stdlib.h>
#include <Cilent/global.h>
#include <Cilent/Assets/Sound.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Misc/File.h>
#include <Cilent/vendor/cute_headers/cute_tiled.h>

Cilent_AssetManager* Cilent_AssetManager_Create()
{
    Cilent_AssetManager* assetManager = malloc(sizeof(Cilent_AssetManager));
    memset(assetManager, 0, sizeof(Cilent_AssetManager));
    
    assetManager->fonts.size = sizeof(int);
    assetManager->fonts.type = "fonts";
    
    assetManager->scripts.size = sizeof(char*);
    assetManager->scripts.type = "scripts";
    
    assetManager->shaders.size = sizeof(Sprender_Shader);
    assetManager->shaders.type = "shaders";
    
    assetManager->sounds.size = sizeof(Cilent_Sound);
    assetManager->sounds.type = "sounds";
    
    assetManager->textures.size = sizeof(Sprender_Texture);
    assetManager->textures.type = "textures";
    
    assetManager->tiled.size = sizeof(cute_tiled_map_t*);
    assetManager->tiled.type = "tiled";
    
    return assetManager;
}

void Cilent_AssetManager_Load(
    Cilent_AssetManager_Type* assets,
    ini_t* modData,
    const char* modName,
    void (*callable)(const char*, const char*, void*)
)
{
    CILENT_ASSERT(assets != NULL);
    CILENT_ASSERT(modData != NULL);
    CILENT_ASSERT(modName != NULL);
    CILENT_ASSERT(callable != NULL);
    
    // Get texture directory
    const char* directoryFmt = "data/%s/%s";
    const size_t directoryLength = (
        strlen(directoryFmt)
        + strlen(modName)
        + strlen(assets->type)
        - 4 // replacements
        + 1 // null terminator
    );
    char* directory = malloc(
        sizeof(char) * directoryLength
    );
    snprintf(directory, directoryLength, directoryFmt, modName, assets->type);
    
    {
        FILE* directoryExists = fopen(directory, "r");
        if (directoryExists == NULL)
        {
            free(directory);
            // TODO: this should return something
            return;
        }
        fclose(directoryExists);
    }
    
    struct dirent** list;
    int listLength;
    listLength = scandir(directory, &list, NULL, alphasort);
    
    assets->count = listLength;
    assets->list = malloc(sizeof(assets->size) * listLength);
    assets->names = malloc(sizeof(char*) * listLength);
    
    // TODO: make this recursive to include assets within directories
    int result;
    int countSoFar = 0;
    char* filepath = NULL;
    while (listLength-- > 0)
    {
        const char* filename = list[listLength]->d_name;
        
        if (filepath != NULL)
        {
            filepath = realloc(
                filepath,
                sizeof(char) * (
                    directoryLength
                    + strlen(filename)
                    + 1 // slash joining character
                    + 1 // null terminator
                )
            );
        }
        else
        {
            filepath = malloc(
                sizeof(char) * (
                    directoryLength
                    + strlen(filename)
                    + 1 // slash joining character
                    + 1 // null terminator
                )
            );
        }
        
        if (
            strcmp(".", filename) == 0
            || strcmp("..", filename) == 0
        )
        {
            free(list[listLength]);
            continue;
        }
        
        sprintf(filepath, "%s/%s", directory, filename);
        
        if (
            !ini_sget(
                modData,
                assets->type,
                filename,
                "%d",
                &result
            )
            || !result
        )
        {
            free(list[listLength]);
            continue;
        }
        
        callable(
            filepath,
            filename,
            &(assets->list[countSoFar * assets->size])
        );
        
        char* name = malloc(sizeof(char) * (strlen(filename) + 1));
        strcpy(name, filename);
        assets->names[countSoFar] = name;
        
        countSoFar++;
        
        free(list[listLength]);
    }
    
    // TODO: resize array & map?
    assets->count = countSoFar;
    
    if (filepath != NULL)
    {
        free(filepath);
    }
    
    free(list);
    free(directory);
}

void Cilent_AssetManager_Load_Font(
    const char* filename,
    const char* key,
    void* ptr
)
{
    int* font = (int*)ptr;
    
    *font = fonsAddFont(
        cilent->fons,
        key,
        filename
    );
    
    debug_log("Loaded font `%s`", key);
}

void Cilent_AssetManager_Load_Script(
    const char* filename,
    const char* key,
    void* ptr
)
{
    char** script = (char**)ptr;
    
    *script = Cilent_File_Read(filename);
    lua_getglobal(cilent->lua, "steps");
    if (luaL_dostring(cilent->lua, *script) != LUA_OK)
    {
        debug_log_type(
            ERROR,
            "%s",
            lua_tostring(cilent->lua, -1)
        );
    }
    
    debug_log("Loaded script `%s`", key);
}

void Cilent_AssetManager_Load_Shader(
    const char* filename,
    const char* key,
    void* ptr
)
{
    Sprender_Shader* shader = (Sprender_Shader*)ptr;
    
    *shader = Sprender_Shader_Load(
        cilent->sprender->fna3d.device,
        filename,
        NULL
    );
    
    debug_log("Loaded shader `%s`", key);
}

void Cilent_AssetManager_Load_Sound(
    const char* filename,
    const char* key,
    void* ptr
)
{
    Wav* wav = Wav_create();
    Wav_load(wav, filename);
    
    Cilent_Sound* sound = (Cilent_Sound*)ptr;
    sound->ptr = wav;
    
    debug_log("Loaded sound `%s`", key);
}

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    const char* key,
    void* ptr
)
{
    Sprender_Texture* texture = (Sprender_Texture*)ptr;
    
    *texture = Sprender_Texture_Load(
        cilent->sprender->fna3d.device,
        filename
    );
    
    debug_log("Loaded texture `%s`", key);
}

void Cilent_AssetManager_Load_Tiled(
    const char* filename,
    const char* key,
    void* ptr
)
{
    cute_tiled_map_t** tiled = (cute_tiled_map_t**)ptr;
    
    *tiled = cute_tiled_load_map_from_file(filename, 0);
    
    debug_log("Tile width: %d", (*tiled)->tilewidth);
    
    debug_log("Loaded tiled map `%s`", key);
}

void* Cilent_AssetManagerType_FindByKey(
    Cilent_AssetManager_Type* assetManagerType,
    const char* key
)
{
    CILENT_ASSERT(assetManagerType != NULL);
    
    for (int i = 0; i < assetManagerType->count; i++) {
        if (strcmp(key, assetManagerType->names[i]) != 0) {
            continue;
        }
        
        return &assetManagerType->list[i];
    }
    
    return NULL;
}

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager)
{
    if (assetManager == NULL) {
        return;
    }
    
    // Free fonts
    for (int i = 0; i < assetManager->fonts.count; i++) {
        // FontStash takes care of freeing the font files
        free(assetManager->fonts.names[i]);
    }
    free(assetManager->fonts.names);
    free(assetManager->fonts.list);
    
    // Free lua scripts
    for (int i = 0; i < assetManager->scripts.count; i++) {
        // Just a char*
        free(assetManager->scripts.list[i]);
        free(assetManager->scripts.names[i]);
    }
    free(assetManager->scripts.names);
    free(assetManager->scripts.list);
    
    // Free shaders
    for (int i = 0; i < assetManager->shaders.count; i++) {
        Sprender_Shader_Destroy(
            cilent->sprender->fna3d.device,
            (Sprender_Shader*)(&assetManager->shaders.list[i])
        );
        free(assetManager->shaders.names[i]);
    }
    free(assetManager->shaders.names);
    free(assetManager->shaders.list);
    
    // Free sounds
    for (int i = 0; i < assetManager->sounds.count; i++) {
        Wav_destroy(((Cilent_Sound*)&assetManager->sounds.list[i])->ptr);
        free(assetManager->sounds.names[i]);
    }
    free(assetManager->sounds.names);
    free(assetManager->sounds.list);
    
    // Free textures
    for (int i = 0; i < assetManager->textures.count; i++) {
        Sprender_Texture_Destroy(
            cilent->sprender->fna3d.device,
            (Sprender_Texture*)(&assetManager->textures.list[i])
        );
        free(assetManager->textures.names[i]);
    }
    free(assetManager->textures.names);
    free(assetManager->textures.list);
    
    // Free tiled maps
    for (int i = 0; i < assetManager->tiled.count; i++) {
        cute_tiled_free_map(assetManager->tiled.list[i]);
        free(assetManager->tiled.names[i]);
    }
    free(assetManager->tiled.names);
    free(assetManager->tiled.list);
    
    // Free the asset manager itself
    free(assetManager);
}
