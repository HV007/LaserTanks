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
    std::cout << "Health Created: " << x << " " << y << "\n";
}

void Health::render(SDL_Renderer* renderer, Texture& mHealthTexture) {

    for(std::pair<std::pair<int,int>, int> drop : drops) {
        int x = drop.first.first;
        int y = drop.first.second;
        if(drops[{x,y}] == 0) continue;
        mHealthTexture.setAlpha(drop.second);
        mHealthTexture.render(renderer, BX + x*GAP + 10, BY + TEXT_GAP + y*GAP + 10);
        if(drop.second < 255) drops[{drop.first.first, drop.first.second}]++;
    }
}

bool Health::hasHealth(int xx, int yy) {
    bool found = false;
    for(std::pair<std::pair<int,int>, int> drop : drops) {
        int x = drop.first.first;
        int y = drop.first.second;
        int xxx = xx - (BX + x*GAP + 10);
        int yyy = yy - (BY + TEXT_GAP + y*GAP + 10);
        if(xxx >= 0 && xxx <= 20 && yyy >= 0 && yyy <= 20 && drops[{x,y}] != 0) {
            found = true;
            drops[{x,y}] = 0;
        }
    }
    return found;
}

int Health::getNextTick() {
    return nextTick;
}