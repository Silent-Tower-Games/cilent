#include <SDL2/SDL.h>
#include <stdio.h>
#include <Cilent/Misc/Assert.h>
#include "Config.h"
#include "../../vendor/ini-master/src/ini.h"

#define CILENT_CONFIG_LOAD_STRING(header, name, max) { \
    const char* strTemp = ini_get( \
        configIni, \
        header, \
        #name \
    ); \
    if (strTemp != NULL && strTemp[0] != '\0') { \
        snprintf(config->name, max + 1, "%s", strTemp); \
    } \
}

#define CILENT_CONFIG_LOAD_INT(header, name) { \
    int intTemp; \
    if (ini_sget( \
        configIni, \
        header, \
        #name, \
        "%d", \
        &intTemp \
    )) { \
        config->name = intTemp; \
    } \
}

static char Cilent_Config_Filename[1024] = "\0";

static char Cilent_Config_LanguageIsValid(char* lang)
{
    int i = 0;

    while (lang[++i] != '\0')
    {
        // If we're on the 6th non-null terminating character, the string is too long
        if (i == 5)
        {
            return 0;
        }

        // If any character isn't a-z or A-Z or a dash, it's an invalid character
        if (
            lang[i] != 45
            && !(lang[i] >= 97 && lang[i] <= 122)
            && !(lang[i] >= 65 && lang[i] <= 90)
        )
        {
            return 0;
        }
    }

    return 1;
}

static char* Cilent_Config_GetFilename(Cilent_Config* config)
{
    if (Cilent_Config_Filename[0] == '\0')
    {
        char* prefPath = SDL_GetPrefPath(
            config->org,
            config->app
        );
        sprintf(
            Cilent_Config_Filename,
            "%sconfig.ini",
            prefPath
        );
        SDL_free(prefPath);
    }
    
    return Cilent_Config_Filename;
}

static char Cilent_Config_Load(Cilent_Config* config, ini_t* configIni)
{
    if (configIni == NULL)
    {
        return 0;
    }
    
    CILENT_CONFIG_LOAD_STRING("", language, 5);
    CILENT_CONFIG_LOAD_INT("", debug);
    CILENT_CONFIG_LOAD_INT("display", vsync);
    CILENT_CONFIG_LOAD_INT("display", loopType);
    CILENT_CONFIG_LOAD_STRING("", game, 127);
    
    return 1;
}

/**
 * @brief Get an allocated INI-formatted list of active addons.
 *
 * Return value must be freed.
 * 
 * @param modState mod state from config
 * @return char* active addons in INI format
 */
static char* Cilent_Config_FileData_ModState_List(Cilent_ModState* modState)
{
    char* data = malloc(
        sizeof(char) * (
            (
                127 // mod name length
                + 3 // length of "=1\n"; "\n" is one character
                + 1 // null terminator
            ) * modState->activeAddonsCount // number of active addons
        )
    );
    
    int length = 0;
    for (int i = 0; i < modState->activeAddonsCount; i++)
    {
        snprintf(
            &data[length],
            127 + 3 + 1,
            "%s=1\n",
            modState->activeAddons[i]->name
        );
        
        length += strlen(modState->activeAddons[i]->name) + 3;
    }
    data[length] = '\0';
    
    return data;
}

/**
 * @brief Get the config as an allocated INI-formatted string.
 * 
 * @param config config state
 * @return char* config state in INI format
 */
static char* Cilent_Config_FileData(Cilent_Config* config)
{
    const char* fmt = (
        "language=%s\n"
        "debug=%d\n"
        "game=%s\n"
        "\n"
        "[display]\n"
        "vsync=%d\n"
        "loopType=%d\n"
        "\n"
        "[addons]\n"
        "%s\n"
        "[testing]\n"
    );
    char* modStateList = Cilent_Config_FileData_ModState_List(&config->modState);
    char* data = malloc(
        sizeof(char) * (
            (strlen(fmt) + 1) // format string
            - 12 // format standins
            + 5 // language
            + 1 // debug
            + strlen(config->game) // game
            + strlen(modStateList) // addons
            + 1 // vsync
            + 1 // loop type
            + 1 // null terminator
        )
    );
    
    int loopType = config->loopType;
    if (loopType < 0 || loopType > 9) {
        loopType = 0;
    }
    
    sprintf(
        data,
        fmt,
        config->language,
        config->debug ? 1 : 0, // only 0 or 1
        config->game,
        config->vsync ? 1 : 0, // only 0 or 1
        loopType,
        modStateList
    );
    free(modStateList);
    
    return data;
}

Cilent_Config Cilent_Config_Create(Cilent_Config configDefault)
{
    Cilent_Config config = configDefault;
    
    // Make sure the given language isn't in an invalid format
    CILENT_ASSERT(Cilent_Config_LanguageIsValid(configDefault.language));
    
    // Load the config INI file
    ini_t* configIni = ini_load(Cilent_Config_GetFilename(&config));
    Cilent_Config_Load(&config, configIni);
    
    // Create the mod state, which also loads all mods
    config.modState = Cilent_ModState_Load(config.game, configIni, config.language);
    
    // After having loaded the INI file, close & free it
    ini_free(configIni);
    
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

void Cilent_Config_Destroy(Cilent_Config* config)
{
    Cilent_ModState_Destroy(config->modState);
}
