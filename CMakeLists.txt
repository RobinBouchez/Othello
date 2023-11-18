cmake_minimum_required(VERSION 3.26)
project(Othello C)

set(CMAKE_C_STANDARD 17)

find_package(SDL2 REQUIRED SDL2)
include_directories(Othello ${SDL2_INCLUDE_DIRS})

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(Othello
        GUI.c
        GUI.h
        Board.c
        Board.h
        Stone.c
        Stone.h
        Pawn.c
        Pawn.h
)

target_link_libraries(Othello ${SDL2_LIBRARIES})