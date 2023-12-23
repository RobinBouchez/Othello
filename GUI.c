#include "GUI.h"

static SDL_Renderer *renderer;
static SDL_Window *window;

//---------------------------------------------------------------------------
// INPUT
//---------------------------------------------------------------------------

int is_relevant_event(SDL_Event *event) {
	if (event == NULL) {
		return 0;
	}
	return  (event->type == SDL_MOUSEBUTTONDOWN) ||
			(event->type == SDL_KEYDOWN) ||
			(event->type == SDL_QUIT);
}

void read_input(void) {
	SDL_Event event;

	while (!SDL_PollEvent(&event) || !is_relevant_event(&event)) {}
	switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_o:;
                char* string = (player_stone == white) ? "White" : "Black";
                printf("%s has skipped a turn.\n", string);
                player_stone = invert_stone(player_stone);
                break;
            case SDLK_p:;
                char* player = (player_stone == white) ? "white" : "black";
                printf("Current player is %s.\n", player);
                 break;
            case SDLK_q:
                should_continue = false;
			}
			break;
		case SDL_QUIT:
            count_stones(main_board);
			should_continue = false;
			break;
		case SDL_MOUSEBUTTONDOWN:;
			int mouse_x = event.button.x;
			int mouse_y = event.button.y;
            int x_pos = mouse_x - (mouse_x % IMAGE_WIDTH);
            int y_pos = mouse_y - (mouse_y % IMAGE_HEIGHT);
            create_pawn(main_board, x_pos, y_pos);
            break;
		}
}

//---------------------------------------------------------------------------
// DRAWING
//---------------------------------------------------------------------------

SDL_Texture* create_stone_texture(const eStone stone) {
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;

    switch (stone) {
        case black:
            surface = SDL_LoadBMP("Images/black.bmp");
            break;
        case white:
            surface = SDL_LoadBMP("Images/white.bmp");
            break;
        default:
            surface= SDL_LoadBMP("Images/empty.bmp");
            break;
    }
    if (surface == NULL) {
        printf("ERROR: Cannot find images!");
        exit(EXIT_FAILURE);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* get_stone_texture(const eStone stone) {
    SDL_Texture *texture = NULL;
    switch (stone) {
        case black:
            texture = black_stone_texture;
            break;
        case white:
            texture = white_stone_texture;
            break;
        default:
            texture = empty_stone_texture;
            break;
    }
    return texture;
}

void clear_window(void) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void draw_rectangle(SDL_Texture *image, unsigned int x, unsigned int y, const unsigned int width, const unsigned int height) {
	SDL_Rect rectangle = { x, y, width, height };
	SDL_RenderCopy(renderer, image, NULL, &rectangle);
}

void draw_board(void)  {
    int x = 0;
    int y = 0;
    for (int i = 0; i < boardWidth; ++i) {
        for (int j = 0; j < boardHeight; ++j) {
            draw_rectangle(get_stone_texture(main_board[i][j]), x, y, IMAGE_WIDTH, IMAGE_HEIGHT);
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

//---------------------------------------------------------------------------
// FREEING MEMORY
//---------------------------------------------------------------------------

void destroy_textures(void) {
    SDL_DestroyTexture(white_stone_texture);
    SDL_DestroyTexture(black_stone_texture);
    SDL_DestroyTexture(empty_stone_texture);
}

void free_board(void) {
    destroy_textures();
    for (int i = 0; i < boardWidth; ++i) {
        free(main_board[i]);
    }
    free(main_board);
}

void free_gui(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//---------------------------------------------------------------------------
// INITIALIZING
//---------------------------------------------------------------------------

void initialize_window(const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error while initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}


void initialize(void) {
    windowWidth = boardWidth * IMAGE_WIDTH;
    windowHeight = boardHeight * IMAGE_HEIGHT;
    initialize_window(WINDOW_TITLE);

    white_stone_texture = create_stone_texture(white);
    black_stone_texture = create_stone_texture(black);
    empty_stone_texture = create_stone_texture(empty);
}

char* get_arguments(int argc, char *argv[]) {
    char* file = NULL;
    if (argc > 1) {
        for (int i = 0; i < argc - 1; ++i) {
            printf("%i: Processing argument: %s\n", i, argv[i]);
            if (strcmp(argv[i], "-f") == 0) {
                file = argv[i + 1];
                printf("   Found -f option with file: %s\n", file);
            }
            if (strcmp(argv[i], "-w") == 0) {
                boardWidth = atoi(argv[i + 1]);
                printf("   Found -w option with width: %d\n", boardWidth);
            }
            if (strcmp(argv[i], "-h") == 0) {
                boardHeight = atoi(argv[i + 1]);
                printf("   Found -h option with height: %d\n", boardHeight);
            }
        }
    }
    if(file != NULL) {
        printf("Loading board from file: %s \n", file);
        main_board = load_board_from_file(file);
        return file;
    }

    if(boardHeight != 0 && boardWidth != 0) {
        printf("Creating an empty board\n");
        main_board = create_empty_board(boardWidth, boardHeight);
    } else {
        printf("Loading board from %s\n", BOARD_FILE);
        main_board = load_board_from_file(BOARD_FILE);
    }
    return BOARD_FILE;
}


void create_pawn(eStone** board, const int x, const int y) {
    int x_pos =  x / IMAGE_WIDTH;
    int y_pos = y / IMAGE_HEIGHT;
    if (is_valid_move(board, player_stone, x_pos, y_pos)) {

        board[x_pos][y_pos] = player_stone;
        flip_stones(main_board, player_stone, x_pos, y_pos);

        //change turn
        player_stone = invert_stone(player_stone);

        save_board(main_file);
        draw();
    }
}

int main(int argc, char *argv[]) {
    main_file = get_arguments(argc, argv);
    initialize();
    draw();

    while (should_continue) read_input();

    free_board();
    free_gui();
    return 0;
}