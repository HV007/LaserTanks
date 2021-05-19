#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "Main.h"

class Tank;
class Network;
class Bullet;

class Player {
public:
    Player(int id);

    void handleEvent(int a, int b, int c, std::vector<Bullet*> &bullets, Mix_Chunk *gBulletSound);
    void move(Maze& maze, Health& h, Network& network, int my_id, Mix_Chunk *gHealthPickSound);
    void reduceHealth();
    void increaseHealth();
    int getHealth();
    int getNextTick();
    void render(SDL_Renderer *renderer, Texture mPlayerTexture[], Texture mTankTexture[], Texture& mBulletTexture, std::map<int,std::string>& idToName);
    int getX();
    int getY();
    void bulletHit();
    bool isDead();
    void setHealth(int h);

private:
    Tank* tank;
    int health;
    int id;
    int nextTick;
};

#endif