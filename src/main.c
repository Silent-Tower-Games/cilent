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
#include <flecs.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    debug_log("%s", Cilent_HelloWorld());
    
    ecs_os_init();
    
    cilent = Cilent_Create((Cilent_Config) {
        .app = "My Game",
        .debug = 0,
        .language = "en",
        .org = "Silent Tower Games",
        .game = "base",
    });
    
    Sprender_SpriteBatch* spriteBatch = Sprender_SpriteBatch_Create(
        cilent->sprender->fna3d.device,
        10000, // 10k
        SPRENDER_SPRITEBATCH_INDEXBUFFER_PREBUILD // use an index buffer
    );
    
    Cilent_Mod* mod = map_get(
        cilent->config.modState.map,
        "floor-is-lava",
        Cilent_Mod
    );
    Sprender_Shader* shader = map_get(
        mod->assetManager->shaders.map,
        "RedShader.fxb",
        Sprender_Shader
    );
    Sprender_Load(
        cilent->sprender,
        NULL,
        shader,
        1
    );
    Sprender_Texture* texture = map_get(
        cilent->config.modState.activeGame->assetManager->textures.map,
        "easter-16x16.png",
        Sprender_Texture
    );
    Sprender_SpriteBatch_Begin(
        spriteBatch,
        texture
    );
    Sprender_SpriteBatch_StageFrame(
        spriteBatch,
        (Sprender_Int2D){ .X = 0, .Y = 0 },
        (Sprender_Float2D){ .X = 0, .Y = 0 },
        (Sprender_Float2D){ .X = 1, .Y = 1 },
        0.0f,
        0,
        0xFFFFFFFF
    );
    Sprender_SpriteBatch_End(spriteBatch);
    Sprender_RenderSprites(cilent->sprender, spriteBatch);
    Sprender_Close(cilent->sprender);
    
    SDL_Delay(5000);
    
    Sprender_SpriteBatch_Destroy(spriteBatch);
    Cilent_Destroy(cilent);
    
    debug_log("Goodbye, world!");
    
    return 0;
}
