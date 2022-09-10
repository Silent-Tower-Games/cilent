#include "Lang.h"

Cilent_Lang* Cilent_Lang_Load(char* mod, char* language)
{
    assert(strnlen(language, 5) <= 5);
    
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
    
    return lang;
}

char* Cilent_Lang_Get(Cilent_Lang* lang, char* section, char* key)
{
    char* value = ini_get(lang->data, section, key);
    
    return value;
}

void Cilent_Lang_Destroy(Cilent_Lang* lang)
{
    ini_free(lang->data);
    free(lang);
}
