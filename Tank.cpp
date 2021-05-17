#include "Tank.h"

enum direction {left,up,right,down};

Tank::Tank() {
    mPosX = GAP + BX + 10;
    mPosY = GAP + BY + TEXT_GAP + 10;
    face = right;
    degree = 0;
    bullets.clear();

    mVelX = 0;
    mVelY = 0;
}

void Tank::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face == left) mVelX -= TANK_VEL;
                else if (face == right) mVelX += TANK_VEL;
                else if (face == up) mVelY -= TANK_VEL;
                else mVelY += TANK_VEL;
                break;
            case SDLK_UP: face = up; break;
            case SDLK_DOWN: face = down; break;
            case SDLK_LEFT: face = left; break;
            case SDLK_RIGHT: face = right; break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face==left) mVelX += TANK_VEL;
                else if (face==right) mVelX -= TANK_VEL;
                else if (face==up) mVelY += TANK_VEL;
                else mVelY -= TANK_VEL;
                break;
            case SDLK_SPACE:
                fire();                // need to improve this(decide timing of bullet, space down or up)
                break;
        }
    }
}

void Tank::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze& maze, Health& health, int& h) {
    for(int i = 0; i < std::abs(mVelX); i++) {
        if(mVelX > 0) mPosX++;
        else mPosX--;
        if(mPosX < 0 || mPosX + TANK_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX, mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH, mPosY+TANK_HEIGHT)) {
            if(mVelX > 0) mPosX--;
            else mPosX++;
            break;
        }
        if(health.hasHealth(mPosX+TANK_WIDTH/2, mPosY+TANK_HEIGHT/2)) h = 100;
    }
    for(int i = 0; i < std::abs(mVelY); i++) {
        if(mVelY > 0) mPosY++;
        else mPosY--;
        if(mPosY < 0 || mPosY + TANK_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX, mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH, mPosY+TANK_HEIGHT)) {
            if(mVelY > 0) mPosY--;
            else mPosY++;
            break;
        }
        if(health.hasHealth(mPosX+TANK_WIDTH/2, mPosY+TANK_HEIGHT/2)) h = 100;
    }

    int tot_bullets=bullets.size();
    int counter=0;
    while(counter<tot_bullets){
        bullets[counter]->move(SCREEN_WIDTH, SCREEN_HEIGHT, maze);
        bool active=bullets[counter]->active;
        if (active) counter++;
        else{
            auto it=bullets.begin()+counter;
            bullets.erase(it);
            tot_bullets--;
        }
    }
}

void Tank::render(SDL_Renderer* renderer, Texture &mTankTexture, Texture &mBulletTexture) {
    if(face == up) degree = 0;
    else if(face == right) degree = 90;
    else if(face == down) degree = 180;
    else degree = 270;
	mTankTexture.render(renderer, mPosX, mPosY, NULL, degree);
    int tot_bullets=bullets.size();
    for(int i=0;i<tot_bullets;i++){
        bullets[i]->render(renderer,mBulletTexture);
    }
}

void Tank::fire(){
    Bullet* bullet=new Bullet(mPosX+7,mPosY+7,face);  
    bullets.push_back(bullet);
}