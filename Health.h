#ifndef HEALTH_H
#define HEALTH_H
#pragma once
#include "Main.h"

class Texture;

class Health {
public:
    Health();
    Health(int xsize, int ysize);

    void initialise();
    void generate();
    void render(SDL_Renderer* renderer, Texture& mTankTexture);

    bool hasHealth(int x, int y);

    int getNextTick();

private:
    int nextTick, generated;
    int xsize, ysize;
    std::map<std::pair<int, int>, int>  drops;
};

#endif