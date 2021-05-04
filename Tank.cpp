#include "Tank.h"

enum direction {left,up,right,down};

Tank::Tank() {
    mPosX = GAP+BX;
    mPosY = GAP+BY;
    face=right;
    degree=0;

    mVelX = 0;
    mVelY = 0;
}

void Tank::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face==left) mVelX-=TANK_VEL;
                else if (face==right) mVelX+=TANK_VEL;
                else if (face==up) mVelY-=TANK_VEL;
                else mVelY+=TANK_VEL;
                break;
            case SDLK_UP: face=up; break;
            case SDLK_DOWN: face=down; break;
            case SDLK_LEFT: face=left; break;
            case SDLK_RIGHT: face=right; break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face==left) mVelX+=TANK_VEL;
                else if (face==right) mVelX-=TANK_VEL;
                else if (face==up) mVelY+=TANK_VEL;
                else mVelY-=TANK_VEL;
                break;
            
        }
    }
}

void Tank::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze) {
    for(int i = 0; i < std::abs(mVelX); i++) {
        if(mVelX > 0) mPosX++;
        else mPosX--;
        if(mPosX < 0 || mPosX + TANK_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH,mPosY+TANK_HEIGHT)) {
            if(mVelX > 0) mPosX--;
            else mPosX++;
            break;
        }
    }
    for(int i = 0; i < std::abs(mVelY); i++) {
        if(mVelY > 0) mPosY++;
        else mPosY--;
        if(mPosY < 0 || mPosY + TANK_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH,mPosY+TANK_HEIGHT)) {
            if(mVelY > 0) mPosY--;
            else mPosY++;
            break;
        }
    }
}

void Tank::render(SDL_Renderer* renderer, Texture &mTankTexture) {
    if (face==left) degree=0;
    else if (face==up) degree=90;
    else if (face==right) degree=180;
    else degree=270;
	mTankTexture.render(renderer, mPosX, mPosY, NULL, degree);
}