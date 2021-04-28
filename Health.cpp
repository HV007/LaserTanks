#include "Health.h"

Health::Health(): xsize(MAZEX), ysize(MAZEY) {}

Health::Health(int xsize, int ysize): xsize(xsize), ysize(ysize) {}

void Health::initialise() {
    nextTick = 3500; generated = 0;
    drops.clear();
}

void Health::generate() {
    generated++;
    nextTick += generated*HEALTH_DROP_INCREMENT;
    int x = rand()%xsize;
    int y = rand()%ysize;
    if(x == 0 || y == 0 || x == xsize-1 || y == ysize-1) return;
    if(drops.find({x,y}) == drops.end() || drops[{x,y}] == 0) drops[{x,y}] = 1;
    std::cout << "HERE: " << x << " " << y << "\n";
}

void Health::render(SDL_Renderer* renderer, Texture& mTankTexture) {

    for(std::pair<std::pair<int,int>, int> drop : drops) {
        int x = 2*(drop.first.first - 1) + 1;
        int y = 2*(drop.first.second - 1) + 1;
        mTankTexture.setAlpha(drop.second);
        mTankTexture.render(renderer, BX + x*GAP, BY + y*GAP);
        if(drop.second < 255) drops[{drop.first.first, drop.first.second}]++;
    }
}

bool Health::hasHealth(int x, int y) {
    return drops.find({x,y}) != drops.end() && drops[{x,y}] != 0;
}

void Health::capture(int x, int y) {
    drops[{x,y}] = 0;
}

int Health::getNextTick() {
    return nextTick;
}