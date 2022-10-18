#include "File.h"
#include <stdio.h>
#include <Cilent/Misc/Assert.h>
#include <SDL2/SDL.h>

char* Cilent_File_Read(const char* filename)
{
    FILE* f = fopen(filename, "r");
    char* data;
    
    CILENT_ASSERT(f != NULL);
    
    fseek(f, 0, SEEK_END);
    uint32_t length = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = malloc(sizeof(char) * (length + 1));
    fread(data, 1, length, f);
    fclose(f);
    
    data[length] = '\0';
    
    return data;
}
