#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define BHEIGHT 20
#define BWIDTH 10

typedef enum EColor {CLR_NULL, CLR_CYAN, CLR_RED} Color;
typedef enum EBlockState {BLK_EMPTY, BLK_STILL, BLK_MOVING} BlockState;
typedef enum EShape {SHP_Z, SHP_S, SHP_I, SHP_O, SHP_L, SHP_J, SHP_T} Shape;

typedef struct SBlock {
    int i,j;
    BlockState state;
    Color color;
} Block;

typedef struct SPiece {
    //Block* blk[4][4];
    int blk[4][4];
    int ic, jc;
    Color color;
    Shape shape;
    int instantFall;
} Piece;

typedef struct SGameState {
    Block* board[BHEIGHT][BWIDTH];
    Piece *nextPiece;
    Piece *currPiece;
    unsigned int timer;
    int level;
    int score;
    int shortLoop;
    int running;
    unsigned int timer2;
} GameState;

#endif // GAME_H_INCLUDED
