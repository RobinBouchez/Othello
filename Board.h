#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <stdbool.h>
#include "Pawn.h"


struct board {
    enum EStone stone;
    SDL_Texture* texture;
};
typedef struct board Board;

int boardWidth;
int boardHeight;

Board*** main_board;
enum EStone boardSide;


char* read_char(FILE* pFile);
Board*** load_board_from_file(const char*);
void check_winner(unsigned int, unsigned int);
void count_stones(Board***);

bool is_valid_move(Board***, enum EStone, int , int);
bool check_horizontal(enum EStone, Board*** ,int, int);
bool check_vertical(enum EStone, Board*** ,int, int);
bool check_diagonal(enum EStone, Board*** ,int, int);


#endif //OTHELLO_BOARD_H
