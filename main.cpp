#include <SDL.h>
#include <iostream>

#include <rpppConfig.h>


#ifdef USE_MODULE_1
    #include "TestLib.h"
#endif

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int arc, char ** argv) {

    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        
        SDL_CreateWindow(
            "SDL2 Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        
        SDL_Delay(2000);
    }
    const std::string inputName = "Mr. Anderson";

    #ifdef USE_MODULE_1
        greetings(inputName);
    #else
        std::cout << " 'ellooooo from " << inputName << " main.cpp" << std::endl;
    #endif  

    return 0;
}
