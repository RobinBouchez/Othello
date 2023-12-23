#include "Board.h"

unsigned int boardWidth = 0;
unsigned int boardHeight = 0;

eStone** main_board;
eStone player_stone;

//---------------------------------------------------------------------------
// SAVING AND CREATING THE BOARD
//---------------------------------------------------------------------------

char* read_char(FILE* pFile) {
    const int max_size = 100;
    char character[max_size];
    return fgets(character, max_size, pFile);
}

eStone** load_board_from_file(const char *filepath) {

    FILE *pFile;
    if ((pFile = fopen(filepath, "r")) == NULL) {

        pFile = fopen(filepath, "w");
        fclose(pFile);
        main_board = create_empty_board(8, 8);
        save_board(filepath);
        return main_board;
    }

    boardWidth = atoi(read_char(pFile));
    boardHeight = atoi(read_char(pFile));
    player_stone = (unsigned char)(read_char(pFile))[0];
    player_stone = (player_stone == WHITE_CHAR) ? white : black;

    //allocate 2D array of pointers
    eStone** new_board = (eStone**)malloc((boardWidth * sizeof(eStone*)));
    for (int i = 0; i < boardWidth; ++i) {
        new_board[i] = (eStone*)malloc((boardHeight * sizeof(eStone)));
    }

    // Read character pieces from file
    char stone;
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            stone = (char)fgetc(pFile);
            eStone selectedStone = (stone == WHITE_CHAR) ? white :
                                   (stone == BLACK_CHAR) ? black : empty;
            new_board[i][j] = selectedStone;
        }
        fgetc(pFile);  // Skip newline
    }

    fclose(pFile);
    return new_board;
}

void save_board(const char *filepath) {
    FILE *pFile = fopen(filepath, "w"); //open de FILE in write mode
    if (pFile == NULL) {
        printf("%s can't be opened \n", filepath);
        exit(EXIT_FAILURE);
    }

    fprintf(pFile, "%i\n", boardWidth);
    fprintf(pFile, "%i\n", boardHeight);
    fprintf(pFile, "%c", (player_stone == white) ? WHITE_CHAR : BLACK_CHAR);

    for (int i = 0; i < boardWidth; ++i) {
        fprintf(pFile, "\n");
        for (int j = 0; j < boardHeight; ++j) {
            fprintf(pFile, "%c",
                    (main_board[i][j] == white) ? WHITE_CHAR :
                    (main_board[i][j] == black) ? BLACK_CHAR : EMPTY_CHAR);
        }
    }
    fclose(pFile);
}

eStone** create_empty_board(const unsigned int width, const unsigned int height) {
    boardWidth = width;
    boardHeight = height;
    eStone stone;
    eStone** new_board = (eStone**)malloc((width * sizeof(eStone*)));

    for (int i = 0; i < width; ++i) {
        new_board[i] = (eStone*)malloc((height * sizeof(eStone)));
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if ((i == width / 2 && j == height / 2) ||
                (i == width / 2 - 1 && j == height / 2 - 1)) {
                stone = white;
            } else if ((i == width / 2 - 1 && j == height / 2) ||
                       (i == width / 2 && j == height / 2 - 1)) {
                stone = black;
            } else {
                stone = empty;
            }
            new_board[i][j] = stone;
        }
    }
    return new_board;
}

//---------------------------------------------------------------------------
// RULES LOGIC
//---------------------------------------------------------------------------

int is_within_bounds(const unsigned int x, const unsigned int y) {
    return x >= 0 && x < boardWidth && y >= 0 && y < boardHeight;
}

bool check_direction(const eStone pawn, eStone** board, const unsigned int x, const unsigned int y, const int xDir, const int yDir) {
    int i = 1;
    eStone opposite = invert_stone(pawn);

    while (is_within_bounds(x + i * xDir, y + i * yDir)  &&
           board[x + i * xDir][y + i * yDir] != empty) {
        if(board[x + 1 * xDir][y + 1 * yDir] != opposite) return false;
        if (board[x + i * xDir][y + i * yDir] == pawn)
            return true;
        i++;
    }
    return false;
}

bool check_vertical(const eStone pawn, eStone** board, const unsigned int x, const unsigned int y) {
    return check_direction(pawn, board, x, y, 0, 1) || check_direction(pawn, board, x, y, 0, -1);
}

bool check_horizontal(const eStone pawn, eStone** board, const unsigned int x, const unsigned int y) {
    return check_direction(pawn, board, x, y, 1, 0) || check_direction(pawn, board, x, y, -1, 0);
}

bool check_diagonal(const eStone pawn, eStone** board, const unsigned int x, const unsigned int y) {
    return check_direction(pawn, board, x, y, 1, 1) || check_direction(pawn, board, x, y, 1, -1) ||
           check_direction(pawn, board, x, y, -1, 1) || check_direction(pawn, board, x, y, -1, -1);
}

bool is_valid_move(eStone** board, const eStone player, const unsigned int x, const unsigned int y) {
    if (board[x][y] != empty) return false;

    return check_vertical(player, board, x, y) ||
               check_horizontal(player, board, x, y) ||
               check_diagonal(player, board, x, y);
}

void check_winner(const unsigned  int white, const unsigned int black) {
    //if (white + black != boardWidth * boardHeight) return;
    if (white > black) {
        printf("White won! \n");
    } else if (white == black) {
        printf("It's a draw! \n");
    } else {
        printf("Black won! \n");
    }
}

void count_stones(eStone** board) {
    unsigned int white_stones = 0;
    unsigned int black_stones = 0;

    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            switch (board[i][j]) {
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
    printf("White stones: %u\n", white_stones);
    printf("Black stones: %u\n", black_stones);
    check_winner(white_stones, black_stones);
}

//---------------------------------------------------------------------------
// FLIPPING STONES
//---------------------------------------------------------------------------

void flip_in_direction(eStone** board, const eStone player, const unsigned int x, const unsigned int y, const int xDir, const int yDir) {
    eStone opposite = invert_stone(player);
    int i = 1;
    int j = 1;

    while (is_within_bounds(x + j * xDir, y + j * yDir) &&
           board[x + j * xDir][y + j * yDir] == opposite) {
        j++;
    }

    if (is_within_bounds(x + j * xDir, y + j * yDir) &&
        board[x + j * xDir][y + j * yDir] == player) {
        while (is_within_bounds(x + i * xDir, y + i * yDir) &&
               board[x + i * xDir][y + i * yDir] == opposite) {
            board[x + i * xDir][y + i * yDir] = player;
            i++;
        }
    }
}

void flip_stones(eStone** board, const eStone player, const unsigned int x, const unsigned int y) {
    // Check in all directions
    for (int xDir = -1; xDir <= 1; ++xDir) {
        for (int yDir = -1; yDir <= 1; ++yDir) {
            if (xDir == 0 && yDir == 0) continue;  // Skip the case where both directions are 0 (no direction)
            flip_in_direction(board, player, x, y, xDir, yDir);
        }
    }
}
