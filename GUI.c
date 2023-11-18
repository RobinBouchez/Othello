#include "GUI.h"

static SDL_Renderer *renderer;
static SDL_Window *window;

int is_relevant_event(SDL_Event *event) {
	if (event == NULL) {
		return 0;
	}
	return  (event->type == SDL_MOUSEBUTTONDOWN) ||
			(event->type == SDL_KEYDOWN) ||
			(event->type == SDL_QUIT);
}

SDL_Texture* create_stone_texture(enum EStone stone) {
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;

    switch (stone) {
        case black:
            surface = SDL_LoadBMP("Images/black.bmp");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            break;
        case white:
            surface = SDL_LoadBMP("Images/white.bmp");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            break;
        default:
            surface= SDL_LoadBMP("Images/empty.bmp");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            break;
    }
    SDL_FreeSurface(surface);
    return texture;
}

void read_input(void) {
	SDL_Event event;

	while (!SDL_PollEvent(&event) || !is_relevant_event(&event)) {}
	switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_o:
                turn_skipped = true;
                printf("Turn skipped\n");
			    break;
            case SDLK_s:
                save_board("Board.txt");
                 break;
            case SDLK_q:
                exit(0);
			}
			break;
		case SDL_QUIT:
            count_stones(main_board);
			should_continue = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse_x = event.button.x;
			mouse_y = event.button.y;
            int x_pos = mouse_x - (mouse_x % IMAGE_WIDTH);
            int y_pos = mouse_y - (mouse_y % IMAGE_HEIGHT);
            create_pawn(main_board, x_pos, y_pos);
            break;
		}
}

void clear_window(void) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void draw_rectangle(SDL_Texture *image, int x, int y, int width, int height) {
	SDL_Rect rectangle = { x, y, width, height };
	SDL_RenderCopy(renderer, image, NULL, &rectangle);
}

void draw_board(void)  {
    int x = 0;
    int y = 0;
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            draw_rectangle(main_board[i][j]->texture, x, y, IMAGE_WIDTH, IMAGE_HEIGHT);
            y += IMAGE_HEIGHT;
        }
        x += IMAGE_WIDTH;
        y=0;
    }
}

void draw(void) {
	clear_window();
    draw_board();
	SDL_RenderPresent(renderer);
}

void initialize_window(const char *title) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error while initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Error: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

void destroy_textures(void) {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            SDL_DestroyTexture(main_board[j][i]->texture);
        }
    }
}

void free_board(void) {
    destroy_textures();
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            free(main_board[i][j]);
        }
    }
    for (int i = 0; i < boardHeight; ++i) {
        free(main_board[i]);
    }
    free(main_board);
}

void free_gui(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void initialize(void) {
    windowWidth = boardWidth * IMAGE_WIDTH;
    windowHeight = boardHeight * IMAGE_HEIGHT;
    player_stone = boardSide;
    initialize_window("Othello");
    init_board();
}

void get_arguments(int argc, char *argv[]) {
    if (argc > 1) {
        for (int i = 0; i < argc; ++i) {
            if (0 == strcmp(argv[i], "-f")) {
                main_board = load_board_from_file(argv[i + 1]);
            }
            if (0 == strcmp(argv[i], "-w")) {
                boardHeight = atoi(argv[i + 1]);
                printf("%i", boardWidth);

            }
            if (0 == strcmp(argv[i], "-h")) {
                boardHeight = atoi(argv[i + 1]);
                printf("%i", boardHeight);
            }
        }
    } else {
        main_board = load_board_from_file("Board.txt");
    }
}

void init_board(void) {
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            switch (main_board[i][j]->stone) {
                case black:
                    main_board[i][j]->texture = create_stone_texture(black);
                    break;
                case white:
                    main_board[i][j]->texture = create_stone_texture(white);
                    break;
                default:
                    main_board[i][j]->texture = create_stone_texture(empty);
                    break;
            }
        }
    }
}


void save_board(const char *filepath) {
    FILE *pFile = NULL;
    pFile = fopen(filepath, "w"); //open de FILE in write mode

    fprintf(pFile, "%i\n", boardWidth);
    fprintf(pFile, "%i\n", boardHeight);
    fprintf(pFile, "%c", (player_stone == white) ? 'W' : 'B'); // als de speler zijn steen wit is

    for (int i = 0; i < boardWidth; ++i) {
        fprintf(pFile, "\n");
        for (int j = 0; j < boardHeight; ++j) {
            fprintf(pFile, "%c",
                    (main_board[i][j]->stone == white) ? 'W' :
                    (main_board[i][j]->stone == black) ? 'B' : 'X');
        }
    }
    fclose(pFile);
}


void flip_pawns(Board*** board, enum EStone player, const int x, const int y) {
    enum EStone opposite = (player == white) ? black : white;

    int i = 1;
    int j = 1;

    while (board[x + j][y]->stone != empty && board[x + j][y]->stone == opposite) {
        j++;
    }
    if (board[x + j][y]->stone == player) {
        while (board[x + i][y]->stone != empty && board[x + i][y]->stone == opposite) {
            board[x + i][y]->stone = player;
            board[x + i][y]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x - j][y]->stone != empty && board[x - j][y]->stone == opposite) {
        j++;
    }
    if (board[x - j][y]->stone == player) {
        while (board[x - i][y]->stone != empty && board[x - i][y]->stone == opposite) {
            board[x - i][y]->stone = player;
            board[x - i][y]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x][y+j]->stone != empty && board[x][y+j]->stone == opposite) {
        j++;
    }
    if (board[x][y+j]->stone == player) {
        while (board[x][y + i]->stone != empty && board[x][y + i]->stone == opposite) {
            board[x][y + i]->stone = player;
            board[x][y + i]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x][y-j]->stone != empty && board[x][y-j]->stone == opposite) {
        j++;
    }
    if (board[x][y-j]->stone == player) {
        while (board[x][y - i]->stone != empty && board[x][y - i]->stone == opposite) {
            board[x][y - i]->stone = player;
            board[x][y - i]->texture = create_stone_texture(player);
            i++;
        }
    }

    //diagonal
    i=1;
    j=1;
    while (board[x + j][y+j]->stone != empty && board[x + j][y+j]->stone == opposite) {
        j++;
    }
    if (board[x + j][y+j]->stone == player) {
        while (board[x + i][y+i]->stone != empty && board[x + i][y+i]->stone == opposite) {
            board[x + i][y+i]->stone = player;
            board[x + i][y+i]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x + j][y-j]->stone != empty && board[x + j][y-j]->stone == opposite) {
        j++;
    }
    if (board[x + j][y-j]->stone == player) {
        while (board[x + i][y-i]->stone != empty && board[x + i][y-i]->stone == opposite) {
            board[x + i][y-i]->stone = player;
            board[x + i][y-i]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x - j][y-j]->stone != empty && board[x - j][y-j]->stone == opposite) {
        j++;
    }
    if (board[x - j][y-j]->stone == player) {
        while (board[x - i][y-i]->stone != empty && board[x - i][y-i]->stone == opposite) {
            board[x - i][y-i]->stone = player;
            board[x - i][y-i]->texture = create_stone_texture(player);
            i++;
        }
    }
    i=1;
    j=1;
    while (board[x - j][y+j]->stone != empty && board[x - j][y+j]->stone == opposite) {
        j++;
    }
    if (board[x - j][y+j]->stone == player) {
        while (board[x - i][y+i]->stone != empty && board[x - i][y+i]->stone == opposite) {
            board[x - i][y+i]->stone = player;
            board[x - i][y+i]->texture = create_stone_texture(player);
            i++;
        }
    }
}

void create_pawn(Board*** array, const int x, const int y) {
    if (is_valid_move(array, player_stone, x / IMAGE_WIDTH, y / IMAGE_HEIGHT)) {

        array[x / IMAGE_WIDTH][y / IMAGE_HEIGHT]->stone = player_stone;
        array[x / IMAGE_WIDTH][y / IMAGE_HEIGHT]->texture = create_stone_texture(player_stone);
        flip_pawns(main_board, player_stone, x / IMAGE_WIDTH, y / IMAGE_HEIGHT);
        if (!turn_skipped) player_stone = (player_stone == white) ? black : white;
        turn_skipped = false;
    }
}

int main(int argc, char *argv[]) {
    get_arguments(argc, argv);
    initialize();

    while (should_continue) {
        draw();
        read_input();
    }
    free_board();
    free_gui();
    return 0;
}