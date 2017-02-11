#include "graphics.h"

#define ALPHA 0.2

GraphState* InitGraphics() {
    GraphState *grs = NULL;
    grs = (GraphState*) malloc(sizeof(GraphState));
    grs->win = NULL;
    grs->ren = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "SDL initialized");

    grs->win = SDL_CreateWindow("Tetris3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
    if(grs->win == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Couldn't create window: %s\n", SDL_GetError());
        free(grs);
        grs = NULL;
        SDL_Quit();

        return NULL;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Window created");

    grs->ren = SDL_CreateRenderer(grs->win, -1, SDL_RENDERER_ACCELERATED);
    if(grs->ren == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Couldn't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(grs->win);
        free(grs);
        grs = NULL;
        SDL_Quit();

        return NULL;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Renderer created");

    return grs;
}

void StopGraphics(GraphState *grs) {
    SDL_DestroyRenderer(grs->ren);
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Renderer destroyed");

    SDL_DestroyWindow(grs->win);
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Window destroyed");

    SDL_Quit();
    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "SDL quit");

    free(grs);
    grs = NULL;
}

void RenderGraphics(GameState *gs, GraphState *grs) {
    SDL_Rect playzone;
    int i,j;

    SDL_SetRenderDrawColor(grs->ren, 128,128,128,255);
    SDL_RenderClear(grs->ren);

    playzone.x = 2*BLKSIZE;
    playzone.y = 2*BLKSIZE;
    playzone.h = BHEIGHT*BLKSIZE;
    playzone.w = BWIDTH*BLKSIZE;
    SDL_RenderSetViewport(grs->ren, &playzone);

    for(i=0; i<BHEIGHT; i++) {
        for(j=0; j<BWIDTH; j++) {
            SDL_Rect block;

            block.x = j*BLKSIZE;
            block.y = i*BLKSIZE;
            block.h = BLKSIZE;
            block.w = BLKSIZE;

            if(gs->board[i][j]->state != BLK_EMPTY) {
                switch(gs->board[i][j]->color) {
                case CLR_CYAN:
                    SDL_SetRenderDrawColor(grs->ren,142+ALPHA*(255-142),251+ALPHA*(255-251),(1-ALPHA)*255,255); // 142 251 255 // 255 0 255
                    break;
                case CLR_RED:
                    //if(SDL_TICKS_PASSED(SDL_GetTicks(), gs->timer2+REFRESHRATE)) {
                       SDL_SetRenderDrawColor(grs->ren,255,255,0,255);
                   // } else {
                    //   SDL_SetRenderDrawColor(grs->ren,255,255,0,255);
                   // }
                    break;
                default:
                    SDL_SetRenderDrawColor(grs->ren,0,0,0,255);
                }
            }
            else {
                SDL_SetRenderDrawColor(grs->ren,255,255,255,255);
            }
            SDL_RenderFillRect(grs->ren, &block);
        }
    }
    if(SDL_TICKS_PASSED(SDL_GetTicks(), gs->timer2+REFRESHRATE)) {
        gs->timer2 = SDL_GetTicks();
    }

    SDL_RenderPresent(grs->ren);
}
