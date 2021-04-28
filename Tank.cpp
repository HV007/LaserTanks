#include "Tank.h"

Tank::Tank() {
    mPosX = GAP+BX;
    mPosY = GAP+BY;

    mVelX = 0;
    mVelY = 0;
}

void Tank::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_UP: mVelY -= TANK_VEL; break;
            case SDLK_DOWN: mVelY += TANK_VEL; break;
            case SDLK_LEFT: mVelX -= TANK_VEL; break;
            case SDLK_RIGHT: mVelX += TANK_VEL; break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_UP: mVelY += TANK_VEL; break;
            case SDLK_DOWN: mVelY -= TANK_VEL; break;
            case SDLK_LEFT: mVelX += TANK_VEL; break;
            case SDLK_RIGHT: mVelX -= TANK_VEL; break;
        }
    }
}

void Tank::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze) {
    mPosX += mVelX;

    if(mPosX < 0 || mPosX + TANK_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX+TANK_WIDTH,mPosY) || maze.check_wall(mPosX,mPosY)) mPosX -= mVelX;
    mPosY += mVelY;
    if(mPosY < 0 || mPosY + TANK_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX,mPosY+TANK_HEIGHT) || maze.check_wall(mPosX,mPosY)) {
        mPosY -= mVelY;
    }
}

void Tank::render(SDL_Renderer* renderer, Texture &mTankTexture) {
	mTankTexture.render(renderer, mPosX, mPosY);
}