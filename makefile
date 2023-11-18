
all: GUI.o Board.o Pawn.o Stone.o
	gcc GUI.o Board.o Pawn.o Stone.o -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2  -o Othello

GUI.o: GUI.c GUI.h Board.h
	gcc GUI.c -c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o GUI.o

Board.o: Board.c Board.h Pawn.h
	gcc Board.c -c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o Board.o

Pawn.o: Pawn.c Pawn.h Stone.h
	gcc Pawn.c -c -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -o Pawn.o

Stone.o: Stone.c Stone.h
	gcc Stone.c -c -o Stone.o