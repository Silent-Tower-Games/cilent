#define _DEFAULT_SOURCE
#include <Cilent/Cilent.h>
#define CILENT_GLOBAL_FINAL
#include <Cilent/global.h>
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
#include <flecs.h>
#include <stdio.h>

Cilent_Mod* mod;
Sprender_Shader* shader;
Sprender_SpriteBatch* spriteBatch;
Sprender_Texture* texture;
Sprender_Float2D position = { .X = 0, .Y = 0 };

int Cilent_Game_Loop();
void MoveSystem();
void DrawSystem();

typedef struct LittleGuy
{
    char player;
    Sprender_Int2D frame;
    Sprender_Float2D position;
} LittleGuy;

ECS_COMPONENT_DECLARE(LittleGuy);

int main(int argc, char** argv)
{
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
    
    spriteBatch = Sprender_SpriteBatch_Create(
        cilent->sprender->fna3d.device,
        10000, // 10k
        SPRENDER_SPRITEBATCH_INDEXBUFFER_PREBUILD // use an index buffer
    );
    mod = map_get(
        cilent->config.modState.map,
        "floor-is-lava",
        Cilent_Mod
    );
    shader = mod == NULL || mod->assetManager == NULL ? NULL : map_get(
        mod->assetManager->shaders.map,
        "RedShader.fxb",
        Sprender_Shader
    );
    texture = map_get(
        cilent->config.modState.activeGame->assetManager->textures.map,
        "easter-16x16.png",
        Sprender_Texture
    );
    texture->tilesize.X = 16;
    texture->tilesize.Y = 16;
    
    ECS_COMPONENT_DEFINE(cilent->world, LittleGuy);
    ECS_SYSTEM(cilent->world, MoveSystem, EcsOnUpdate, LittleGuy);
    ECS_SYSTEM(cilent->world, DrawSystem, EcsOnUpdate, LittleGuy);
    
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
    
    Cilent_Loop(cilent);
    
    Sprender_SpriteBatch_Destroy(spriteBatch);
    Cilent_Destroy(cilent);
    
    debug_log("Goodbye, world!");
    
    return 0;
}

int Cilent_Game_Loop()
{
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
        }
        
        inputEvent(event);
    }
    
    inputPreframe();
    
    ecs_progress(cilent->world, 0);
    
    inputPostframe();
    
    return quit;
}

void MoveSystem(const ecs_iter_t* it)
{
    LittleGuy* littleGuy = ecs_field(it, LittleGuy, 1);
    const float speed = 1.0f;
    
    for (int i = 0; i < it->count; i++) {
        if (!littleGuy[i].player) {
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
        shader == NULL ? 0 : 1
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
    Sprender_Close(cilent->sprender);
}
