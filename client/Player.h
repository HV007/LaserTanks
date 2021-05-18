#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "Main.h"

class Tank;
class Network;

class Player {
public:
    Player(int id);

    void handleEvent(int a, int b, int c);
    void move(Maze& maze, Health& h, Network& network, int my_id);
    void reduceHealth();
    void increaseHealth();
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