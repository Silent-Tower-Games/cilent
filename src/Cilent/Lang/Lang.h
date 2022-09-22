#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <flecs.h>
#include <vendor/ini-master/src/ini.h>

typedef struct Cilent_Lang
{
    char language[6];
    ini_t* data;
} Cilent_Lang;

Cilent_Lang* Cilent_Lang_Load(char* mod, char* language);

const char* Cilent_Lang_Get(Cilent_Lang* lang, const char* section, const char* key);

void Cilent_Lang_Destroy(Cilent_Lang* lang);
