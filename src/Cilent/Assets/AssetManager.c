#include "AssetManager.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <Cilent/Misc/Assert.h>

Cilent_AssetManager* Cilent_AssetManager_Create()
{
    Cilent_AssetManager* assetManager = malloc(sizeof(Cilent_AssetManager));
    memset(assetManager, 0, sizeof(Cilent_AssetManager));
    
    return assetManager;
}

void Cilent_AssetManager_Load(
    Cilent_AssetManager* assetManager,
    ini_t* modData,
    const char* modName,
    const char* assetType,
    void (*callable)(void*)
)
{
    CILENT_ASSERT(assetManager != NULL);
    CILENT_ASSERT(modData != NULL);
    CILENT_ASSERT(modName != NULL);
    CILENT_ASSERT(assetType != NULL);
    CILENT_ASSERT(callable != NULL);
    
    // Get texture directory
    const char* directoryFmt = "data/%s/%s";
    const size_t directoryLength = (
        strlen(directoryFmt)
        + strlen(modName)
        + strlen(assetType)
        - 4 // replacements
        + 1 // null terminator
    );
    char* directory = malloc(
        sizeof(char) * directoryLength
    );
    snprintf(directory, directoryLength, directoryFmt, modName, assetType);
    
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
    
    printf("Mod `%s` list `%s` length: %d\n", modName, assetType, listLength);
    
    // TODO: make this recursive to include assets within directories
    int result;
    while (listLength-- > 0)
    {
        const char* filename = list[listLength]->d_name;
        printf("Asset: %s\n", filename);
        
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
                assetType,
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
    }
    
    free(list);
    free(directory);
}

void Cilent_AssetManager_Load_Texture(
    const char* filename,
    Sprender_Texture* texture
)
{
    // TODO
}

void Cilent_AssetManager_Destroy(Cilent_AssetManager* assetManager)
{
    free(assetManager);
}
