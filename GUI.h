#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <SDL2/SDL.h>

#include "Board.h"

//-----------------------------
// CONSTANTS
//-----------------------------

#define IMAGE_HEIGHT 50
#define IMAGE_WIDTH 50
#define WINDOW_TITLE "Othello"
#define BOARD_FILE "Board.txt"

//-----------------------------
// VARIABLES
//-----------------------------

int windowHeight;
int windowWidth;

bool should_continue = true;

char* main_file;

SDL_Texture* black_stone_texture;
SDL_Texture* white_stone_texture;
SDL_Texture* empty_stone_texture;

//-----------------------------
// FUNCTION DECLARATIONS
//-----------------------------

void create_pawn(eStone**, int, int);

#endif /* GUI_H_ */
