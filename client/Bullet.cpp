#include "Bullet.h"

enum direction {left,up,right,down};

Bullet::Bullet(int x,int y, int f) {
    mPosX = x;
    mPosY = y;
    face=f;   
    if (face==up) degree=0;                // depending on the image of bullet
    else if (face==right) degree=90;
    else if (face==down) degree=180;
    else degree=270;  
    active=true;
    mVelX=0,mVelY=0;
    if (face==right) mVelX=BULLET_VEL;
    else if (face==left) mVelX=-BULLET_VEL;
    else if (face==up) mVelY=-BULLET_VEL;
    else mVelY=BULLET_VEL;
}

void Bullet::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze) {
    for(int i = 0; i < std::abs(mVelX); i++) {
        if(mVelX > 0) mPosX++;
        else mPosX--;
        if(mPosX < 0 || mPosX + BULLET_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+BULLET_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+BULLET_HEIGHT) || maze.check_wall(mPosX+BULLET_WIDTH,mPosY+BULLET_HEIGHT)) {
            active=false;
            break;
        }
    }
    for(int i = 0; i < std::abs(mVelY); i++) {
        if(mVelY > 0) mPosY++;
        else mPosY--;
        if(mPosY < 0 || mPosY + BULLET_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+BULLET_WIDTH, mPosY) || maze.check_wall(mPosX,mPosY+BULLET_HEIGHT) || maze.check_wall(mPosX+BULLET_WIDTH,mPosY+BULLET_HEIGHT)) {
            active=false;
            break;
        }
    }
}

void Bullet::render(SDL_Renderer* renderer, Texture &mBulletTexture) {
	mBulletTexture.render(renderer, mPosX, mPosY, NULL, degree);
}