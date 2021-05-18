#ifndef MAZE_H
#define MAZE_H
#pragma once
#include "Main.h"

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
    void generate(int seed);
    void render(SDL_Renderer* renderer, Uint8 opacity);
    bool check_wall(int x,int y);

private:
    int xsize, ysize;
    cell MAZE[MAX_SIZE][MAX_SIZE];
    int numin;
};
#endif