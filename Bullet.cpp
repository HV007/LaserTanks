#include "Bullet.h"

enum direction {left,up,right,down};

Bullet::Bullet() {
    mPosX = GAP+BX;
    mPosY = GAP+BY;
    face=right;     // this depends on the time when it got fired
    degree=0;

    mVelX = 0;
    mVelY = 0;
}

void Bullet::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face==left) mVelX-=BULLET_VEL;
                else if (face==right) mVelX+=BULLET_VEL;
                else if (face==up) mVelY-=BULLET_VEL;
                else mVelY+=BULLET_VEL;
                break;
            case SDLK_UP: face=up; break;
            case SDLK_DOWN: face=down; break;
            case SDLK_LEFT: face=left; break;
            case SDLK_RIGHT: face=right; break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face==left) mVelX+=BULLET_VEL;
                else if (face==right) mVelX-=BULLET_VEL;
                else if (face==up) mVelY+=BULLET_VEL;
                else mVelY-=BULLET_VEL;
                break;
            
        }
    }
}

void Bullet::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze) {
    for(int i = 0; i < std::abs(mVelX); i++) {
        if(mVelX > 0) mPosX++;
        else mPosX--;
        if(mPosX < 0 || mPosX + BULLET_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+BULLET_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+BULLET_HEIGHT) || maze.check_wall(mPosX+BULLET_WIDTH,mPosY+BULLET_HEIGHT)) {
            if(mVelX > 0) mPosX--;
            else mPosX++;
            break;
        }
    }
    for(int i = 0; i < std::abs(mVelY); i++) {
        if(mVelY > 0) mPosY++;
        else mPosY--;
        if(mPosY < 0 || mPosY + BULLET_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+BULLET_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+BULLET_HEIGHT) || maze.check_wall(mPosX+BULLET_WIDTH,mPosY+BULLET_HEIGHT)) {
            if(mVelY > 0) mPosY--;
            else mPosY++;
            break;
        }
    }
}

void Bullet::render(SDL_Renderer* renderer, Texture &mBulletTexture) {
    if (face==left) degree=0;
    else if (face==up) degree=90;
    else if (face==right) degree=180;
    else degree=270;
	mBulletTexture.render(renderer, mPosX, mPosY, NULL, degree);
}