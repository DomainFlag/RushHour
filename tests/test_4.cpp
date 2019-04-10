#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

int main() {
    // window we'll be rendering to
    SDL_Window * window = NULL;

    // SDL2 font
    TTF_Font * font = NULL;

    // initialize SDL_ttf
    if(TTF_Init() == -1) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();

        return 0;
    } else {
        font = TTF_OpenFont("./calibri/Calibri.ttf", 24);
        if(font == NULL) {
            cout << "TTF_OpenFont: " <<  TTF_GetError() << endl;

            return 0;
        }
    }

    // initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    } else {
        // create window
        window = SDL_CreateWindow("RushHour", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);

        if(window == NULL) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError();
        } else {
            SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

            SDL_Delay(2000);

            SDL_DestroyRenderer(renderer);
        }
    }

    if(font != NULL) {
        // free sdl2 fonts
        TTF_CloseFont(font);
    }

    // destroy window
    SDL_DestroyWindow(window);

    // quit SDL subsystems
    SDL_Quit();

    return 0;
}