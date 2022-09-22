#include <stdio.h>
#include "File.h"

char Cilent_File_Exists(const char* path)
{
    FILE* file = fopen(path, "r");
    
    if (file == NULL) {
        return 0;
    }
    
    fclose(file);
    
    return 1;
}
