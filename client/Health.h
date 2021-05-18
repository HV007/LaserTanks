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
    std::pair<int, int> generate();
    void createHealth(int x, int y);
    void collectHealth(int x, int y);
    void render(SDL_Renderer* renderer, Texture& mTankTexture);

    bool hasHealth(int x, int y);

    int getNextTick();

private:
    int nextTick, generated;
    int xsize, ysize;
    std::map<std::pair<int, int>, int>  drops;
};

#endif