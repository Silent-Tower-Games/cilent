#define _DEFAULT_SOURCE
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>

int main(int argc, char** argv)
{
    printf("%s\n", Cilent_HelloWorld());
    
    struct dirent** list;
    int listLength;
    const char* directory = "data";
    listLength = scandir(directory, &list, NULL, alphasort);
    const int pathLength = 1024;
    char* path = malloc(sizeof(char) * pathLength);
    while(listLength--)
    {
        if(
            strcmp(list[listLength]->d_name, ".") == 0
            ||
            strcmp(list[listLength]->d_name, "..") == 0
        )
        {
            continue;
        }
        
        if(snprintf(path, pathLength, "%s/%s", directory, list[listLength]->d_name) >= pathLength)
        {
            printf("Path name too long (limit: %d characters)! `%s/%s`\n", pathLength - 1, directory, list[listLength]->d_name);
            continue;
        }
        
        struct stat stats;
        stat(path, &stats);
        
        printf("`%s`: %s\n", path, S_ISDIR(stats.st_mode) ? "Directory" : "File");
        
        free(list[listLength]);
    }
    free(path);
    free(list);
    
    return 0;
    
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
}
