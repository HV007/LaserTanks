#ifndef MAZE_H
#define MAZE_H
#pragma once
#include "Main.h"

const int MAX_SIZE = 1000;

struct cell {
    bool in;
    bool up;
    bool left;
    int prevx, prevy;
};

class Maze {
public:
    Maze();
    Maze(int xsize, int ysize);

    void initialise();
    void generate();
    void render(SDL_Renderer* renderer);

private:
    int xsize, ysize;
    cell MAZE[MAX_SIZE][MAX_SIZE];
    int numin;
};
#endif