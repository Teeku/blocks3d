#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdlib.h>

#include "game.h"

int IsOccupied(GameState *gs, int x, int y);
int MovePiece(GameState *gs, Piece *pce, int diri, int dirj);
int RotatePiece(GameState *gs, Piece *pce);

#endif // UTILS_H_INCLUDED
