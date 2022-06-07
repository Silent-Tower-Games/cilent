#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include "Config.h"
#include "../../vendor/ini-master/src/ini.h"

#define CILENT_CONFIG_LOAD_STRING(name) { \
    const char* strTemp = ini_get( \
        configIni, \
        "", \
        #name \
    ); \
    if (strTemp != NULL && strTemp[0] != '\0') { \
        snprintf(config->name, 5, "%s", strTemp); \
    } \
}

#define CILENT_CONFIG_LOAD_INT(name) { \
    int intTemp; \
    if (ini_sget( \
        configIni, \
        "", \
        #name, \
        "%d", \
        &intTemp \
    )) { \
        config->name = intTemp; \
    } \
}

static char* Cilent_Config_Languages[125];

static char Cilent_Config_Filename[1024] = "\0";

static char Cilent_Config_LanguageIsValid(char* lang)
{
    int i = 0;

    while (lang[++i] != '\0') {
        // If we're on the 6th non-null terminating character, the string is too long
        if (i == 5) {
            return 0;
        }

        // If any character isn't a-z or A-Z or a dash, it's an invalid character
        if (
            lang[i] != 45
            && !(lang[i] >= 97 && lang[i] <= 122)
            && !(lang[i] >= 65 && lang[i] <= 90)) {
            return 0;
        }
    }

    return 1;
}

static char* Cilent_Config_GetFilename(Cilent_Config* config)
{
    if (Cilent_Config_Filename[0] == '\0') {
        sprintf(
            Cilent_Config_Filename,
            "%sconfig.ini",
            SDL_GetPrefPath(
                config->org,
                config->app
                
            )
        );
    }
    
    return Cilent_Config_Filename;
}

static char Cilent_Config_Load(Cilent_Config* config)
{
    ini_t* configIni = ini_load(Cilent_Config_GetFilename(config));
    
    if (configIni == NULL) {
        return 0;
    }
    
    CILENT_CONFIG_LOAD_STRING(language);
    
    CILENT_CONFIG_LOAD_INT(debug);
    
    ini_free(configIni);
    
    return 1;
}

Cilent_Config Cilent_Config_Create(Cilent_Config configDefault)
{
    Cilent_Config config = configDefault;
    
    assert(Cilent_Config_LanguageIsValid(configDefault.language));
    
    Cilent_Config_Load(&config);
    
    return config;
}

char Cilent_Config_Save(Cilent_Config* config)
{
    char* data = Cilent_Config_FileData(config);
    
    FILE* configFile = fopen(Cilent_Config_GetFilename(config), "w");
    
    if (configFile == NULL) {
        return 0;
    }
    
    fwrite(
        data,
        sizeof(char),
        strlen(data),
        configFile
    );
    
    fclose(configFile);
    free(data);
    
    return 1;
}

char* Cilent_Config_FileData(Cilent_Config* config)
{
    char* fmt = (
        "language=%s\n"
        "debug=%d\n"
    );
    char* data = malloc(
        sizeof(char) * (
            (strlen(fmt) + 1) // format string
            - 4 // format standins
            + 5 // language
            + 1 // debug
            + 1 // null terminator
        )
    );
    sprintf(
        data,
        fmt,
        config->language,
        config->debug % 2, // only 0 or 1
        config->org,
        config->app
    );
    
    return data;
}
