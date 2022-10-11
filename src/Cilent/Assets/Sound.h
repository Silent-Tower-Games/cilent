#pragma once

typedef enum Cilent_Sound_Type
{
    CILENT_SOUND_SFXR,
    CIELNT_SOUND_WAV,
} Cilent_Sound_Type;

typedef struct Cilent_Sound
{
    Cilent_Sound_Type type;
    void* ptr;
} Cilent_Sound;
