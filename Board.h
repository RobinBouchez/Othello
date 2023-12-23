#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#include <stdbool.h>
#include "SDL2/SDL.h"
#include "Stone.h"

//-----------------------------
// CONSTANTS
//-----------------------------

#define WHITE_CHAR 'W'
#define BLACK_CHAR 'B'
#define EMPTY_CHAR 'X'

//-----------------------------
// VARIABLES
//-----------------------------

extern unsigned int boardWidth;
extern unsigned int boardHeight;

extern eStone** main_board;
extern eStone player_stone;


//-----------------------------
// FUNCTION DECLARATIONS
//-----------------------------

char* read_char(FILE* pFile);

eStone** load_board_from_file(const char*);
eStone** create_empty_board(unsigned int, unsigned int);
void save_board(const char*);

void count_stones(eStone**);

bool is_valid_move(eStone**, eStone, unsigned int , unsigned int);

void flip_stones(eStone**, eStone, unsigned int, unsigned int);

#endif //OTHELLO_BOARD_H
