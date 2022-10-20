#define _DEFAULT_SOURCE
#include <Cilent/Cilent.h>
#define CILENT_GLOBAL_FINAL
#include <Cilent/global.h>
#include <Cilent/Assets/Sound.h>
#include <Cilent/Config/Config.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Lang/Lang.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Misc/Log.h>
#include <Cilent/Mods/ModState.h>
#include <SDL2/SDL.h>
#include <Sprender/Sprender.h>
#include <STGInput/STGInput.h>
#define STGINPUT_CONVENIENCE
#include <STGInput/Convenience.h>
#define FONTSTASH_IMPLEMENTATION
#include <vendor/fontstash/fontstash.h>
#include <Cilent/Flecs/Maps.h>
#include <Cilent/Fonts/FontStash.h>
#include <flecs.h>
#include <stdio.h>

Cilent_Mod* mod;
Cilent_Mod* mod2;
Sprender_Shader* shader;
char shaderEnabled = 0;
Cilent_Sound* sound;
Cilent_Sound* sound2;
Sprender_SpriteBatch* spriteBatch;
Sprender_Texture* texture;
Sprender_Float2D position = { .X = 0, .Y = 0 };

Speech* speech;
Sfxr* sfxr;
char sfxrCooldown = 0;
unsigned int soundInstance = 0;
Noise* noise;
unsigned int noiseFluctuation = 0;
unsigned int noiseInstance = 0;
char pauseSong = 0;

int Cilent_Game_Loop();

void LuaScriptSystem();
void EnableShaderSystem();
void PlaySoundSystem();
void MoveSystem();
void DrawSystem();

void FirstTestScene();

typedef struct LittleGuy
{
    char player;
    Sprender_Int2D frame;
    Sprender_Float2D position;
} LittleGuy;

ECS_COMPONENT_DECLARE(LittleGuy);

typedef struct Holder
{
    int val;
} Holder;

int main(int argc, char** argv)
{
    //*
    Holder** list = malloc(sizeof(Holder) * 2);
    list[0] = malloc(sizeof(Holder));
    list[0]->val = 1;
    list[1] = malloc(sizeof(Holder));
    list[1]->val = 2;
    
    ecs_os_init();
    ecs_map_t* map = map_new(Holder*, 2);
    
    map_set(map, "one", &list[0]);
    map_set(map, "two", &list[1]);
    
    printf(
        "Holders: %d %d\n",
        (*map_get(map, "one", Holder*))->val,
        (*map_get(map, "two", Holder*))->val
    );
    
    list[0]->val = 5;
    list[1]->val = 7;
    
    printf(
        "Holders: %d %d\n",
        (*map_get(map, "one", Holder*))->val,
        (*map_get(map, "two", Holder*))->val
    );
    
    free(list[0]);
    free(list[1]);
    free(list);
    
    return 0;
    //*/
    
    debug_log("%s", Cilent_HelloWorld());
    
    cilent = Cilent_Create(
        (Cilent_Config) {
            .app = "My Game",
            .debug = 0,
            .language = "en",
            .org = "Silent Tower Games",
            .game = "base",
            .loopType = FPSLOOP_TYPE_NOTHING,
            .vsync = 1,
        },
        Cilent_Game_Loop
    );
    cilent->nextScene = FirstTestScene;
    
    spriteBatch = Sprender_SpriteBatch_Create(
        cilent->sprender->fna3d.device,
        10000, // 10k
        SPRENDER_SPRITEBATCH_INDEXBUFFER_PREBUILD // use an index buffer
    );
    cilent->fontStashSprender.spriteBatch = spriteBatch;
    mod = map_get(
        cilent->config.modState.map,
        "floor-is-lava",
        Cilent_Mod
    );
    mod2 = map_get(
        cilent->config.modState.map,
        "higher-jump",
        Cilent_Mod
    );
    noise = Noise_create();
    Noise_setType(noise, NOISE_BROWNISH);
    sfxr = Sfxr_create();
    speech = Speech_create();
    Speech_setText(speech, "Hello World");
    shader = mod == NULL || mod->assetManager == NULL ? NULL : map_get(
        mod->assetManager->shaders.map,
        "RedShader.fxb",
        Sprender_Shader
    );
    sound = map_get(
        cilent->config.modState.activeGame->assetManager->sounds.map,
        "fifth-demo.ogg",
        Cilent_Sound
    );
    sound2 = map_get(
        mod2->assetManager->sounds.map,
        "healthpickup1.ogg",
        Cilent_Sound
    );
    texture = map_get(
        cilent->config.modState.activeGame->assetManager->textures.map,
        "easter-16x16.png",
        Sprender_Texture
    );
    texture->tilesize.X = 16;
    texture->tilesize.Y = 16;
    
    Cilent_Loop(cilent);
    
    Sprender_SpriteBatch_Destroy(spriteBatch);
    Cilent_Destroy(cilent);
    
    debug_log("Goodbye, world!");
    
    return 0;
}

int Cilent_Game_Loop()
{
    if (cilent->nextScene != NULL || cilent->world == NULL)
    {
        if (cilent->world != NULL)
        {
            ecs_fini(cilent->world);
        }
        
        cilent->world = ecs_init();
        
        ECS_COMPONENT_DEFINE(cilent->world, LittleGuy);
        ECS_SYSTEM(cilent->world, LuaScriptSystem, EcsOnUpdate);
        ECS_SYSTEM(cilent->world, EnableShaderSystem, EcsOnUpdate);
        ECS_SYSTEM(cilent->world, PlaySoundSystem, EcsOnUpdate);
        ECS_SYSTEM(cilent->world, MoveSystem, EcsOnUpdate, LittleGuy);
        ECS_SYSTEM(cilent->world, DrawSystem, EcsOnUpdate, LittleGuy);
        
        if (cilent->nextScene != NULL)
        {
            cilent->nextScene();
            cilent->nextScene = NULL;
        }
    }
    
    SDL_Event event;
    char quit = 0;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
            {
                quit = 1;
            } break;
            
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        Cilent_Event_Focus(cilent);
                    } break;
                    
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        Cilent_Event_Blur(cilent);
                    } break;
                }
            } break;
        }
        
        // Handling input
        inputEvent(event);
    }
    
    if (cilent->focus) {
        inputPreframe();
        
        ecs_progress(cilent->world, 0);
        
        inputPostframe();
    }
    
    return quit;
}

void FirstTestScene()
{
    ecs_entity_t player = ecs_new_id(cilent->world);
    ecs_set(cilent->world, player, LittleGuy, {
        .player = 1,
        .frame = { .X = 5, .Y = 0 },
        .position = { .X = 0, .Y = 0 },
    });
    
    ecs_entity_t enemy = ecs_new_id(cilent->world);
    ecs_set(cilent->world, enemy, LittleGuy, {
        .player = 0,
        .frame = { .X = 0, .Y = 0 },
        .position = { .X = 32, .Y = 16 },
    });
}

void LuaScriptSystem()
{
    if (!keyboard(Pressed, l)) {
        return;
    }
    
    lua_getglobal(cilent->lua, "steps");
    if(luaL_dostring(cilent->lua, "for i,v in pairs(steps) do v(); end") != LUA_OK)
    {
        debug_log_type(
            ERROR,
            "Lua: %s",
            lua_tostring(cilent->lua, -1)
        );
    }
}

void EnableShaderSystem()
{
    if (keyboard(Pressed, a)) {
        shaderEnabled = !shaderEnabled;
    }
}

void PlaySoundSystem()
{
    if (sound != NULL && keyboard(Pressed, s)) {
        if (soundInstance) {
            pauseSong = !pauseSong;
            
            debug_log(
                "%f",
                Soloud_getStreamPosition(cilent->soloud, soundInstance)
            );
        } else {
            soundInstance = Soloud_play(cilent->soloud, sound->ptr);
        }
    }
    
    Soloud_setPause(
        cilent->soloud,
        soundInstance,
        pauseSong
    );
    
    if (sound2 != NULL && keyboard(Pressed, f)) {
        Soloud_play(cilent->soloud, sound2->ptr);
    }
    
    if (sfxrCooldown > 0) {
        sfxrCooldown--;
    }
    if (sfxr != NULL && sfxrCooldown <= 0 && keyboard(Down, d)) {
        sfxrCooldown = 10;
        
        Sfxr_loadPreset(sfxr, SFXR_POWERUP, rand());
        
        Soloud_play(cilent->soloud, sfxr);
    }
    
    if (speech != NULL && keyboard(Pressed, g)) {
        Soloud_play(cilent->soloud, speech);
    }
    
    // Sounds like a train :)
    if (noise != NULL && keyboard(Pressed, n)) {
        Soloud_stop(cilent->soloud, noiseInstance);
        
        noiseInstance = Soloud_play(cilent->soloud, noise);
    }
    noiseFluctuation++;
    Soloud_setVolume(
        cilent->soloud,
        noiseInstance,
        0.25f + (0.75f * fabs(sinf((float)noiseFluctuation / 3.0f)))
    );
}

void MoveSystem(const ecs_iter_t* it)
{
    LittleGuy* littleGuy = ecs_field(it, LittleGuy, 1);
    const float speed = 1.0f;
    
    for (int i = 0; i < it->count; i++) {
        if (!littleGuy[i].player) {
            littleGuy[i].position.X += speed;
            continue;
        }
        
        if (keyboard(Down, RIGHT)) {
            littleGuy[i].position.X += speed;
        }
        
        if (keyboard(Down, LEFT)) {
            littleGuy[i].position.X -= speed;
        }
        
        if (keyboard(Down, DOWN)) {
            littleGuy[i].position.Y += speed;
        }
        
        if (keyboard(Down, UP)) {
            littleGuy[i].position.Y -= speed;
        }
    }
}

void DrawSystem(const ecs_iter_t* it)
{
    LittleGuy* littleGuy = ecs_field(it, LittleGuy, 1);
    
    Sprender_Load(
        cilent->sprender,
        NULL,
        shader,
        shader == NULL || !shaderEnabled ? 0 : 1
    );
    Sprender_SpriteBatch_Begin(
        spriteBatch,
        texture
    );
    
    for (int i = 0; i < it->count; i++) {
        Sprender_SpriteBatch_StageFrame(
            spriteBatch,
            littleGuy[i].frame,
            littleGuy[i].position,
            (Sprender_Float2D){ .X = 1, .Y = 1 },
            0.0f,
            0,
            0xFFFFFFFF
        );
    }
    
    Sprender_SpriteBatch_End(spriteBatch);
    Sprender_RenderSprites(cilent->sprender, spriteBatch);
    
    fonsSetFont(cilent->fons, cilent->defaultFont);
    fonsSetSize(cilent->fons, 8.0f);
    fonsSetColor(cilent->fons, 0xFFFF00FF);
    fonsDrawText(
        cilent->fons,
        0,
        0,
        "Testing!",
        NULL
    );
    
    fonsSetFont(cilent->fons, fonsGetFontByName(cilent->fons, "arcade.ttf"));
    fonsSetSize(cilent->fons, 16.0f);
    fonsSetColor(cilent->fons, 0xFF00FFFF);
    fonsDrawText(
        cilent->fons,
        -16,
        -16,
        "Testing!",
        NULL
    );
    
    fonsSetFont(cilent->fons, fonsGetFontByName(cilent->fons, "alagard.ttf"));
    fonsSetSize(cilent->fons, 16.0f);
    fonsSetColor(cilent->fons, 0xFF00FF00);
    fonsDrawText(
        cilent->fons,
        16,
        16,
        "Testing!",
        NULL
    );
    
    Sprender_Close(cilent->sprender);
}
