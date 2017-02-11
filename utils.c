#include "utils.h"

int IsOccupied(GameState *gs, int x, int y) {
    if(x < 0 || x >= BHEIGHT || y < 0 || y >= BWIDTH)
        return 1;

    if(gs->board[x][y]->state == BLK_STILL)
        return 1;

    return 0;
}

int MovePiece(GameState *gs, Piece *pce, int diri, int dirj) {
    int i,j;

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(gs->currPiece->blk[i][j]) {
                if(IsOccupied(gs,gs->currPiece->ic+i+diri,gs->currPiece->jc+j+dirj))
                    return 0;
            }
        }
    }

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pce->blk[i][j]) {
                gs->board[gs->currPiece->ic+i][gs->currPiece->jc+j]->state = BLK_EMPTY;
                gs->board[gs->currPiece->ic+i][gs->currPiece->jc+j]->color = CLR_NULL;
            }
        }
    }

    pce->ic += diri;
    pce->jc += dirj;

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pce->blk[i][j]) {
                gs->board[gs->currPiece->ic+i][gs->currPiece->jc+j]->state = BLK_MOVING;
                gs->board[gs->currPiece->ic+i][gs->currPiece->jc+j]->color = pce->color;
            }
        }
    }

    return 1;
}

int RotatePiece(GameState *gs, Piece *pce) {
    int tmp1[4][4] = {{0}};
    int tmp2[4][4] = {{0}};
    int i,j;
    int tsize;

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pce->blk[j][i]) {
                tmp1[i][j] = 1;
            }
        }
    }

    if(pce->shape == SHP_I) {
        tsize = 3;
    } else {
        tsize = 2;
    }

    for(i=0;i<tsize+1;i++) {
        for(j=0;j<tsize+1;j++) {
            tmp2[i][j] = tmp1[i][tsize-j];
            tmp2[i][j] = tmp1[i][tsize-j];
        }
    }

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(tmp2[i][j])
                if(IsOccupied(gs,pce->ic+i,pce->jc+j))
                    return 0;
        }
    }

    for(i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            if(pce->blk[i][j]) {
                pce->blk[i][j] = 0;
                gs->board[pce->ic+i][pce->jc+j]->state = BLK_EMPTY;
                gs->board[pce->ic+i][pce->jc+j]->color = CLR_NULL;
            }
            if(tmp2[i][j]) {
                pce->blk[i][j] = tmp2[i][j];
                gs->board[pce->ic+i][pce->jc+j]->state = BLK_MOVING;
                gs->board[pce->ic+i][pce->jc+j]->color = pce->color;
            }
        }
    }

    return 1;
}
