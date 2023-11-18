#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <SDL2/SDL.h>

#include "Board.h"

#define IMAGE_HEIGHT 50
#define IMAGE_WIDTH 50

int windowHeight;
int windowWidth;

int mouse_x = 0;
int mouse_y = 0;

int should_continue = 1;

void initialize_window(const char *title);
void read_input(void);
void draw(void);
void free_gui(void);
SDL_Texture* create_stone_texture(enum EStone stone);
void draw_rectangle(SDL_Texture *image, int x, int y, int width, int height);
void destroy_textures();


enum EStone player_stone;

bool turn_skipped;

void create_pawn(Board***, int, int);
void init_board(void);
void save_board(const char*);
void count_stones(Board***);
void flip_pawns(Board*** board, enum EStone, int , int);


#endif /* GUI_H_ */
