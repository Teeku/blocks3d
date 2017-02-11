#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <stdio.h>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "utils.h"

#define GLOOPSLEEP 30
#define BASEFALLTIME 20
#define SHORTLOOP 80

static const int speed[21] = {53,49,45,41,37,33,28,22,17,11,10,9,8,7,6,6,5,5,4,4,3};

void NewGame(GraphState *grs);
void FreeGame(GameState *gs);
void UpdateGameState(GameState *gs);
void GameLoop(GameState *gs, GraphState *grs);
Piece* GeneratePiece(GameState *gs);
void PlacePiece(GameState *gs, Piece *pce);
void DeleteLine(GameState *gs, int l);

int SDL_main(int argc, char** argv)
{
    GraphState *grs = NULL;

    srand(time(NULL));

    grs = InitGraphics();
    if(grs == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize graphics, exiting");
        return EXIT_FAILURE;
    }

    NewGame(grs);

    StopGraphics(grs);

    return EXIT_SUCCESS;
}

void NewGame(GraphState *grs) {
    int i,j;
    GameState *gs = NULL;

    gs = (GameState*) malloc(sizeof(GameState));
    if(gs == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "NewGame - Mem alloc error: GameState");
        return;
    }

    for(i=0; i<BHEIGHT; i++) {
        for(j=0; j<BWIDTH; j++) {
            gs->board[i][j] = (Block*) malloc(sizeof(Block));
            gs->board[i][j]->j = j;
            gs->board[i][j]->i = i;
            gs->board[i][j]->state = BLK_EMPTY;
            gs->board[i][j]->color = CLR_NULL;
        }
    }
    gs->nextPiece = GeneratePiece(gs);
    gs->currPiece = GeneratePiece(gs);
    PlacePiece(gs, gs->currPiece);
    gs->timer = SDL_GetTicks();
    gs->timer2 = SDL_GetTicks();
    gs->level = 1;
    gs->score = 0;
    gs->running = 1;
    gs->shortLoop = 0;

    GameLoop(gs, grs);

    printf("l:%d s:%d\n",gs->level, gs->score),
    system("pause");

    FreeGame(gs);
}

void FreeGame(GameState *gs) {
    int i,j;
    if(gs != NULL) {
        for(i=0; i<BHEIGHT; i++) {
            for(j=0; j<BWIDTH; j++) {
                free(gs->board[i][j]);
            }
        }
        free(gs->nextPiece);
        free(gs->currPiece);

        free(gs);
        gs = NULL;
    }
}

void UpdateGameState(GameState *gs) {
    unsigned int tdiff = SDL_GetTicks() - gs->timer;
    int i,j;
    int linecnt;

    if(SDL_TICKS_PASSED(tdiff,gs->shortLoop?SHORTLOOP:(unsigned int)(BASEFALLTIME*speed[gs->level < 21 ? gs->level : 20]))|| gs->currPiece->instantFall) {

        // Check game over
        for(j=0;j<BWIDTH;j++) {
            if(gs->board[3][j]->state == BLK_STILL) {
                gs->running = 0;
                return;
            }
        }

        if(gs->currPiece->instantFall)
            while(MovePiece(gs, gs->currPiece, 1, 0));

        if(!MovePiece(gs, gs->currPiece, 1, 0)) {
            for(i=0;i<4;i++) {
                for(j=0;j<4;j++) {
                    if(gs->currPiece->blk[i][j])
                        gs->board[gs->currPiece->ic+i][gs->currPiece->jc+j]->state = BLK_STILL;
                }
            }

            free(gs->currPiece);
            gs->currPiece = gs->nextPiece;
            gs->nextPiece = GeneratePiece(gs);

            linecnt = 0;
            for(i=0; i<BHEIGHT; i++) {
                int hole = 0;

                for(j=0; j<BWIDTH; j++) {
                    if(gs->board[i][j]->state != BLK_STILL)
                        hole = 1;
                }
                if(!hole) {
                    DeleteLine(gs, i);
                    linecnt++;
                    gs->score += 50;
                }
            }
            if(linecnt >= 4)
                gs->score += 200;

            if(gs->score > 0 && gs->score % 1000 == 0) {
                gs->score = 0;
                gs->level++;
            }


            PlacePiece(gs, gs->currPiece);
        }


        gs->timer = SDL_GetTicks();
    }

}

void GameLoop(GameState *gs, GraphState *grs) {
    unsigned int currTime;
    while(gs->running) {
        currTime = SDL_GetTicks();

        ProcessInput(gs);
        UpdateGameState(gs);
        RenderGraphics(gs, grs);

        if(!SDL_TICKS_PASSED(SDL_GetTicks()-currTime, GLOOPSLEEP)) {
            SDL_Delay(GLOOPSLEEP - (SDL_GetTicks() - currTime));
        }
    }
}

Piece* GeneratePiece(GameState *gs) {
    const int pieces[7][4][4] = { {{0,0,1,0},
                                   {0,1,1,0},
                                   {0,1,0,0},
                                   {0,0,0,0}},

                                  {{0,1,0,0},
                                   {0,1,1,0},
                                   {0,0,1,0},
                                   {0,0,0,0}},

                                  {{0,0,1,0},
                                   {0,0,1,0},
                                   {0,0,1,0},
                                   {0,0,1,0}},

                                  {{0,1,1,0},
                                   {0,1,1,0},
                                   {0,0,0,0},
                                   {0,0,0,0}},

                                  {{0,1,1,0},
                                   {0,1,0,0},
                                   {0,1,0,0},
                                   {0,0,0,0}},

                                  {{0,1,0,0},
                                   {0,1,0,0},
                                   {0,1,1,0},
                                   {0,0,0,0}},

                                  {{0,1,0,0},
                                   {0,1,1,0},
                                   {0,1,0,0},
                                   {0,0,0,0}} };

    int nb = rand()%7;
    int i,j;
    Piece *pce;
    Color clr = rand()%2+1;

    pce = (Piece*) malloc(sizeof(Piece));

    pce->ic = 0;
    pce->jc = BWIDTH/2-2;
    pce->color = clr;
    pce->shape = nb;
    pce->instantFall = 0;

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pieces[nb][i][j]) {
                pce->blk[i][j] = 1;
            } else {
                pce->blk[i][j] = 0;
            }
        }
    }

    return pce;
}

void PlacePiece(GameState *gs, Piece *pce) {
    int i,j;

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pce->blk[i][j]) {
                gs->board[pce->ic+i][pce->jc+j]->state = BLK_MOVING;
                gs->board[pce->ic+i][pce->jc+j]->color = pce->color;
            }
        }
    }
}

void DeleteLine(GameState *gs, int l) {
    int i,j;
    for(j=0; j<BWIDTH; j++) {
        gs->board[l][j]->state = BLK_EMPTY;
        gs->board[l][j]->color = CLR_NULL;
    }
    for(i=l; i>0; i--) {
        for(j=0; j<BWIDTH; j++) {
            gs->board[i][j]->state = gs->board[i-1][j]->state;
            gs->board[i][j]->color = gs->board[i-1][j]->color;
        }
    }
}
