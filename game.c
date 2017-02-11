#include "game.h"

void NewGame(GameState *gs) {
    int i,j;

    if(gs == NULL) {
        gs = (GameState*) malloc(sizeof(GameState));
        if(gs == NULL) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "NewGame - Mem alloc error: GameState");
            return;
        }
    }

    for(i=0; i<BHEIGHT; i++) {
        for(j=0; j<BWIDTH; j++) {
            gs->board[i][j].x = -1;
            gs->board[i][j].y = -1;
            gs->board[i][j].state = BLK_EMPTY;
            gs->board[i][j].color = CLR_NULL;
        }
    }
    gs->nextPiece = NULL;
    gs->level = 1;
    gs->score = 0;
    gs->running = 1;

    GameLoop(gs);

    FreeGame(gs);
}

void FreeGame(GameState *gs) {
    if(gs != NULL) {
        free(gs);
        gs = NULL;
    }
}

void GameLoop(GameState *gs) {
    while(gs->running) {
        ProcessInput(gs);
        //UpdateGameState(gs);
        //RenderGraphics(gs);
    }
}


