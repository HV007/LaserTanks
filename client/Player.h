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

    void handleEvent(SDL_Event &e, Mix_Chunk *gBulletSound, Network& network, std::vector<Bullet*> &bullets);
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
    void moveTo(int x, int y);
    void setFace(int f);
    void fire(std::vector<Bullet*> &bullets, Mix_Chunk *gBulletSound);

private:
    Tank* tank;
    int health;
    int id;
    int nextTick;
};

#endif