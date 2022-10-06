#include "AssetManager.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <Cilent/global.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Flecs/Maps.h>

Cilent_AssetManager* Cilent_AssetManager_Create()
{
    Cilent_AssetManager* assetManager = malloc(sizeof(Cilent_AssetManager));
    memset(assetManager, 0, sizeof(Cilent_AssetManager));
    
    assetManager->shaders.map = ecs_map_new(Sprender_Shader, 0);
    assetManager->shaders.size = sizeof(Sprender_Shader);
    assetManager->shaders.type = "shaders";
    
    assetManager->textures.map = ecs_map_new(Sprender_Texture, 0);
    assetManager->textures.size = sizeof(Sprender_Texture);
    assetManager->textures.type = "textures";
    
    return assetManager;
}

void Cilent_AssetManager_Load(
    Cilent_AssetManager_Type* assets,
    ini_t* modData,
    const char* modName,
    void (*callable)(const char*, const char*, void*, ecs_map_t*)
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
            // TODO: this should return something
            return;
        }
        fclose(directoryExists);
    }
    
    struct dirent** list;
    int listLength;
    listLength = scandir(directory, &list, NULL, alphasort);
    
    printf("Mod `%s` list `%s` length: %d\n", modName, assets->type, listLength);
    
    assets->count = listLength;
    assets->list = malloc(sizeof(assets->size) * listLength);
    ecs_map_set_size(assets->map, listLength);
    
    // TODO: make this recursive to include assets within directories
    int result;
    int countSoFar = 0;
    char* filepath = NULL;
    while (listLength-- > 0)
    {
        const char* filename = list[listLength]->d_name;
        printf("Asset: %s\n", filename);
        
        // FIXME: freeing & reallocating probably mega-sucks for performance
        // This could be a realloc if I put the time into it, but I've spent
        // too much of my life on this lol it needs to get operational before
        // it gets optimized
        if (filepath != NULL)
        {
            free(filepath);
        }
        
        filepath = malloc(
            sizeof(char) * (
                directoryLength
                + strlen(filename)
                + 1 // slash joining character
                + 1 // null terminator
            )
        );
        sprintf(filepath, "%s/%s", directory, filename);
        
        if (
            strcmp(".", filename) == 0
            || strcmp("..", filename) == 0
        )
        {
            continue;
        }
        
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
            printf("Not loading %s\n", filename);
            continue;
        }
        
        printf("!!! Loading %s\n", filename);
        
        callable(
            filepath,
            filename,
            &(assets->list[countSoFar * assets->size]),
            assets->map
        );
        
        countSoFar++;
    }
    
    // TODO: add textures to map
    // TODO: resize array & map?
    
    free(list);
    free(directory);
}

void Cilent_AssetManager_Load_Shader(
    const char* filename,
    const char* key,
    void* ptr,
    ecs_map_t* map
)
{
    Sprender_Shader* shader = (Sprender_Shader*)ptr;
    
    printf("Loading shader at `%s`\n", filename);
    
    *shader = Sprender_Shader_Load(
        cilent->sprender->fna3d.device,
        filename,
        NULL
    );
    
    map_set(map, key, shader);
    
    printf("Added to map: `%s`\n", key);
}

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    const char* key,
    void* ptr,
    ecs_map_t* map
)
{
    Sprender_Texture* texture = (Sprender_Texture*)ptr;
    
    printf("Loading texture at `%s`\n", filename);
    
    *texture = Sprender_Texture_Load(
        cilent->sprender->fna3d.device,
        filename
    );
    
    map_set(map, key, texture);
    
    printf("Added to map: `%s`\n", key);
}

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager)
{
    free(assetManager);
}
