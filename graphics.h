#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "game.h"

#define WHEIGHT 1000
#define WWIDTH 800

#define BLKSIZE 35
#define BPP 32

#define REFRESHRATE 150


typedef struct SGraphState {
    SDL_Window *win;
    SDL_Renderer *ren;
} GraphState;

GraphState* InitGraphics();
void StopGraphics(GraphState *grs);
void RenderGraphics(GameState *gs, GraphState *grs);

#endif // GRAPHICS_H_INCLUDED
