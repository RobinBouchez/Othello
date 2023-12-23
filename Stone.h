#ifndef OTHELLO_STONE_H
#define OTHELLO_STONE_H


//-----------------------------
// STRUCT
//-----------------------------

enum stone {
    black,
    white,
    empty
};

typedef enum stone eStone;

//-----------------------------
// FUNCTION DECLARATIONS
//-----------------------------

eStone invert_stone(eStone);

#endif //OTHELLO_STONE_H
