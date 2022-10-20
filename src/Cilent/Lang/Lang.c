#include "Lang.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <flecs.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Misc/Log.h>

Cilent_Lang* Cilent_Lang_Load(const char* mod, const char* language)
{
    CILENT_ASSERT(strnlen(language, 5) <= 5);
    
    printf("Loading lang `%s` for `%s`\n", language, mod);
    
    Cilent_Lang* lang = (Cilent_Lang*)malloc(sizeof(Cilent_Lang));
    
    strcpy(
        lang->language,
        language
    );
    
    const char* filenameFormat = "data/%s/lang/%s.ini";
    const size_t filenameLength = (
        strlen(filenameFormat)
        + strlen(mod)
        + strlen(language)
        - 4 // for the two `%s` in the format
        + 1 // null terminator
    );
    char* filename = (char*)malloc(sizeof(char) * filenameLength);
    snprintf(
        filename,
        filenameLength,
        filenameFormat,
        mod,
        language
    );
    
    lang->data = ini_load(filename);
    free(filename);
    
    if (lang->data == NULL)
    {
        printf("Destroying...\n");
        Cilent_Lang_Destroy(lang);
        
        return NULL;
    }
    
    return lang;
}

const char* Cilent_Lang_Get(Cilent_Lang* lang, const char* section, const char* key)
{
    if (lang->data == NULL)
    {
        debug_log_type(
            ERROR,
            "Language not found: `%s`",
            lang->language
        );
        
        return NULL;
    }
    
    const char* value = ini_get(lang->data, section, key);
    
    if (value == NULL) {
        debug_log_type(
            ERROR,
            "Lang key not found: `%s:%s:%s`",
            lang->language,
            section,
            key
        );
    }
    
    return value;
}

void Cilent_Lang_Destroy(Cilent_Lang* lang)
{
    printf("Destroying language\n");
    
    if (lang->data != NULL)
    {
        ini_free(lang->data);
    }
    
    free(lang);
}
