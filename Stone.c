#include "Stone.h"

eStone invert_stone(const eStone stone){
    return (stone == white) ? black : white;
}