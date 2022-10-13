#pragma once

#include <Sprender/Sprender.h>
#include <vendor/fontstash/fontstash.h>

typedef struct FontStashSprender
{
    Sprender* sprender;
    Sprender_Texture texture;
    Sprender_SpriteBatch* spriteBatch;
} FontStashSprender;

void FONS_renderDelete(void* uptr);

int FONS_renderCreate(void* uptr, int width, int height);

int FONS_renderResize(void* uptr, int width, int height);

void FONS_renderUpdate(void* uptr, int* rect, const unsigned char* data);

void FONS_renderDraw(void* uptr, const float* verts, const float* tcoords, const unsigned int* colors, int nverts);
