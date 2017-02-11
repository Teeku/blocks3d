#include "input.h"

void ProcessInput(GameState *gs) {
    SDL_Event event;
    int dir = -1;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_DOWN])
        gs->shortLoop = 1;
    else
        gs->shortLoop = 0;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            LogInput("Quit requested");
            gs->running = 0;
            break;

        case SDL_KEYDOWN:
            LogInput("Key pushed: %s", (event.key.keysym.sym));
            switch(event.key.keysym.sym) {
            case SDLK_RIGHT:
                dir = 1;
            case SDLK_LEFT:
                MovePiece(gs, gs->currPiece, 0, dir);
                break;

            case SDLK_UP:
                RotatePiece(gs, gs->currPiece);
                break;

            case SDLK_SPACE:
                gs->currPiece->instantFall = 1;
                break;

            default:
                break;
            }

        case SDL_KEYUP:
            LogInput("Key released: %s", SDL_GetKeyName(event.key.keysym.sym));
            switch(event.key.keysym.sym) {
            default:
                break;
            }
        }
    }
}
