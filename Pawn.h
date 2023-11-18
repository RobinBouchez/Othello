#ifndef OTHELLO_PAWN_H
#define OTHELLO_PAWN_H

#include "SDL2/SDL.h"
#include "Stone.h"

struct pawn {
    int x;
    int y;
    SDL_Texture* texture;
};
typedef struct pawn Pawn;

#endif //OTHELLO_PAWN_H
