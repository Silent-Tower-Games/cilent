#include <stdio.h>
#include <SDL2/SDL.h>
#include <Cilent/Cilent.h>

int main()
{
    printf("%s\n", Cilent_HelloWorld());
    
    Cilent* cilent = Cilent_Create();
    
    SDL_Delay(1000);
    
    Cilent_Destroy(cilent);
    
    return 0;
}
