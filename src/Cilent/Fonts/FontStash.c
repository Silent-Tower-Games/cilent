#include "FontStash.h"

void FONS_renderDelete(void* uptr)
{
    FontStashSprender* fonsprender = (FontStashSprender*)uptr;
    Sprender_Texture_Destroy(fonsprender->sprender->fna3d.device, &fonsprender->texture);
}

int FONS_renderCreate(void* uptr, int width, int height)
{
    FontStashSprender* fonsprender = (FontStashSprender*)uptr;
    
    if(fonsprender->texture.asset != NULL)
    {
        FONS_renderDelete(uptr);
    }
    
    fonsprender->texture = Sprender_Texture_NewBlank(
        fonsprender->sprender->fna3d.device,
        (FNA3D_Vec4){ 0, 0, 0, 0 },
        width,
        height,
        0
    );
    
    return 1;
}

int FONS_renderResize(void* uptr, int width, int height)
{
    return FONS_renderCreate(uptr, width, height);
}

void FONS_renderUpdate(void* uptr, int* rect, const unsigned char* data)
{
    FontStashSprender* fonsprender = (FontStashSprender*)uptr;
    
    const int
        width = fonsprender->texture.size.X,
        height = fonsprender->texture.size.Y,
        size = width * height * sizeof(unsigned char),
        channels = 4
    ;
    
    unsigned char* _data = malloc(size * channels);
    
    // set data
    for(int i = 0; i < size; i++)
    {
        unsigned int val = data[i];
        
        for(int c = 0; c < channels; c++)
        {
            _data[(i * channels) + c] = val;
        }
    }
    
    // set texture
    FNA3D_SetTextureData2D(
        fonsprender->sprender->fna3d.device,
        fonsprender->texture.asset,
        0,
        0,
        width,
        height,
        0,
        _data,
        size * channels
    );
    
    free(_data);
}

void FONS_renderDraw(
    void* uptr,
    const float* verts,
    const float* tcoords,
    const unsigned int* colors,
    int nverts
)
{
    FontStashSprender* fonsprender = (FontStashSprender*)uptr;
    
    Sprender_SpriteBatch_Begin(
        fonsprender->spriteBatch,
        &fonsprender->texture
    );
    
    for(int i = 0; i < nverts * 2; i += 12)
    {
        Sprender_SpriteBatch_StageQuad(
            fonsprender->spriteBatch,
            (Sprender_Quad){
                .top = tcoords[i + 1],
                .bottom = tcoords[i + 3],
                .left = tcoords[i + 0],
                .right = tcoords[i + 2],
            },
            (Sprender_Quad){
                .top = verts[i + 1],
                .bottom = verts[i + 3],
                .left = verts[i + 0],
                .right = verts[i + 2],
            },
            1.0f,
            colors[i / 2]
        );
    }
    
    Sprender_SpriteBatch_End(fonsprender->spriteBatch);
    
    Sprender_RenderSprites(fonsprender->sprender, fonsprender->spriteBatch);
}
