#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "Main.h"

class Tank;

class Player {
public:
    Player(int id);

    void handleEvent(SDL_Event& e);
    void move(Maze& maze, Health& h);
    void reduceHealth();
    int getHealth();
    int getNextTick();
    void render(SDL_Renderer *renderer, Texture mPlayerTexture[], Texture& mTankTexture);

private:
    Tank* tank;
    int health;
    int id;
    int nextTick;
};

#endif