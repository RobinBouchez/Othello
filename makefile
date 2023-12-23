
all: GUI.o Board.o Stone.o
	gcc GUI.o Board.o Stone.o -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2  -o Othello

GUI.o: GUI.c GUI.h Board.h
	gcc GUI.c -c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o GUI.o

Board.o: Board.c Board.h Stone.h
	gcc Board.c -c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o Board.o

Stone.o: Stone.c Stone.h
	gcc Stone.c -c -o Stone.o