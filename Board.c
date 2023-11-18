#include "Board.h"


char* read_char(FILE* pFile) {
    const int size = 100;
    char character[size];
    return fgets(character, size, pFile);
}

Board*** load_board_from_file(const char *filepath) {
    char* character;
    FILE *pFile;

    pFile = fopen(filepath, "r");

    if (pFile == NULL) {
        printf("%s can't be opened \n", filepath);
        exit(1);
    }

    character = read_char(pFile);
    boardWidth = atoi(character);
    character = read_char(pFile);
    boardHeight = atoi(character);
    character = read_char(pFile);
    boardSide = (unsigned char) *character;

    if (boardSide == 'W') {
        boardSide = white;
    } else if (boardSide == 'B') {
        boardSide = black;
    }

    char stone = (char) fgetc(pFile); //get character from fie
    enum EStone  selectedStone;

    //allocate 2D array of pointers
    Board*** new_board = (Board***)malloc((boardWidth * sizeof(Board**)));

    for (int i = 0; i < boardWidth; ++i) {
        new_board[i] = (Board**)malloc((boardHeight * sizeof(Board*)));
    }

    //read character pieces from file
    do {
        for (int i = 0; i < boardWidth; ++i) {
            for (int j = 0; j < boardHeight; ++j) {
                selectedStone = (stone == 'W') ? white :
                                (stone == 'B') ? black : empty;

                Board* piece = (Board*) malloc(sizeof(Board));
                piece->stone = selectedStone;
                piece->texture = NULL;
                new_board[i][j] = piece;

                stone = (char)fgetc(pFile);
            }
            stone = (char)fgetc(pFile);
        }

    } while (stone != EOF);

    fclose(pFile);

    return new_board;
}


void check_winner(unsigned const int white, unsigned const int black) {
    if (white + black != boardWidth * boardHeight) return;
    if (white > black) {
        printf("White has %i stones and black has %i stones\n", white, black);
        printf("White won! \n");
    } else if (white == black) {
        printf("White has %i stones and black has %i stones! \n", white, black);
        printf("It's a draw! \n");
    } else {
        printf("White has %i stones and black has %i stones!\n", white, black);
        printf("Black won!\n");
    }
}

bool check_vertical(enum EStone pawn, Board*** board, const int x, const int y) {
    enum EStone  opposite = (pawn == white) ? black : white;

    int i = 1;
    while ((x + i) + 1 < boardWidth && board[x+i][y]->stone != empty) {
        if(board[x+i][y]->stone == opposite)
            return true;
        i++;
    }
    i = 1;
    while (x - i > 0 && board[x-i][y]->stone != empty) {
        if(board[x-i][y]->stone == opposite)
            return true;
        i++;
    }
    return false;
}

bool check_horizontal(enum EStone pawn, Board*** board,const int x, const int y) {
    enum EStone opposite = (pawn == white) ? black : white;

    int i = 1;
    while ((y + i) + 1 < boardHeight && board[x][y+i]->stone != empty) {
        if(board[x][y+i]->stone == opposite)
            return true;
        i++;
    }
    i = 1;
    while (y - i > 0 && board[x][y-i]->stone != empty) {
        if(board[x][y-i]->stone == opposite)
            return true;
        i++;
    }
    return false;
}

bool check_diagonal(enum EStone pawn, Board*** board, const int x, const int y) {
    enum EStone opposite = (pawn == white) ? black : white;

    int i = 1;
    while ((y + i) + 1 < boardHeight && (x + i) + 1 < boardHeight && board[x+i][y+i]->stone != empty) {
        if(board[x+i][y+i]->stone == opposite)
            return true;
        i++;
    }
    i = 1;
    while (y - i > 0  && (x + i) + 1 < boardWidth && board[x+i][y-i]->stone != empty) {
        if(board[x+i][y-i]->stone == opposite)
            return true;
        i++;
    }
    i = 1;
    while (x - i > 0  && (y + i) + 1 < boardHeight && board[x-i][y+i]->stone != empty) {
        if(board[x-i][y+i]->stone == opposite)
            return true;
        i++;
    }
    i = 1;
    while (y - i > 0 && x - i > 0 && board[x-i][y-i]->stone != empty) {
        if(board[x-i][y-i]->stone == opposite)
            return true;
        i++;
    }
    return false;
}

bool is_valid_move(Board*** board, enum EStone  player, const int x, const int y) {
    if (board[x][y]->stone != empty) return false;
    enum EStone opposite = (player == white) ? black : white;

    bool valid = ((x + 1 < boardWidth) && board[x + 1][y]->stone == opposite) ||
                 ((x - 1 > 0) && board[x - 1][y]->stone == opposite) ||
                 ((y + 1 < boardHeight) && board[x][y + 1]->stone == opposite) ||
                 ((y - 1 > 0) && board[x][y - 1]->stone == opposite); //||
    //((y + 1 < boardHeight) && board[x + 1][y + 1]->stone == opposite) ||
    //((y + 1 < boardHeight) && board[x + 1][y - 1]->stone == opposite) ||
    //((y + 1 < boardHeight) && board[x - 1][y + 1]->stone == opposite) ||
    // ((y + 1 < boardHeight) && board[x - 1][y - 1]->stone == opposite)

    enum EStone d = board[x - 1][y]->stone;

    if (valid)
        return check_vertical(player, board, x, y) ||
               check_horizontal(player, board, x, y) ||
               check_diagonal(player, board, x, y);

    return false;
}


void count_stones(Board*** board) {
    unsigned int white_stones = 0;
    unsigned int black_stones = 0;

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            switch (board[i][j]->stone) {
                case white:
                    white_stones++;
                    break;
                case black:
                    black_stones++;
                    break;
                default:
                    break;
            }
        }
    }
    // check_winner(white_stones, black_stones);
}
