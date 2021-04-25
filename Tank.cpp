#include "Tank.h"

Tank::Tank() {
    mPosX = 0;
    mPosY = 0;

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

void Tank::move(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    mPosX += mVelX;

    if(mPosX < 0 || mPosX + TANK_WIDTH > SCREEN_WIDTH) mPosX -= mVelX;
    mPosY += mVelY;
    if(mPosY < 0 || mPosY + TANK_HEIGHT > SCREEN_HEIGHT) {
        mPosY -= mVelY;
    }
}

void Tank::render(SDL_Renderer* renderer, Texture &mTankTexture) {
	mTankTexture.render(renderer, mPosX, mPosY);
}