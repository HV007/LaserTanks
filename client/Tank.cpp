#include "Tank.h"

enum direction {left,up,right,down};

Tank::Tank(int x, int y, int f) {
    mPosX = x;
    mPosY = y;
    face = f;
    degree = 0;

    mVelX = 0;
    mVelY = 0;
    delay=0;
    check_delay=3;
    delay_time=0;
}

void Tank::handleEvent(SDL_Event& e, int id, Mix_Chunk *gBulletSound, Network& network, std::vector<Bullet*> &bullets) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                if (face == left) mVelX -= TANK_VEL;
                else if (face == right) mVelX += TANK_VEL;
                else if (face == up) mVelY -= TANK_VEL;
                else mVelY += TANK_VEL;
                break;
            case SDLK_UP: face = up; if(mVelX != 0 || mVelY != 0) {mVelX = 0; mVelY = -TANK_VEL;}; network.sendMessage("9 " + std::to_string(id) + " " + std::to_string(face) + "\n"); break;
            case SDLK_DOWN: face = down; if(mVelX != 0 || mVelY != 0) {mVelX = 0; mVelY = TANK_VEL;}; network.sendMessage("9 " + std::to_string(id) + " " + std::to_string(face) + "\n"); break;
            case SDLK_LEFT: face = left; if(mVelX != 0 || mVelY != 0) {mVelX = -TANK_VEL; mVelY = 0;}; network.sendMessage("9 " + std::to_string(id) + " " + std::to_string(face) + "\n"); break;
            case SDLK_RIGHT: face = right; if(mVelX != 0 || mVelY != 0) {mVelX = TANK_VEL; mVelY = 0;}; network.sendMessage("9 " + std::to_string(id) + " " + std::to_string(face) + "\n"); break;
        }
    }  else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                delay=0;
                check_delay=3;
                delay_time=0;
                if(face == left) mVelX += TANK_VEL;
                else if(face == right) mVelX -= TANK_VEL;
                else if(face == up) mVelY += TANK_VEL;
                else mVelY -= TANK_VEL;
                break;
            case SDLK_SPACE:
                fire(bullets, id, gBulletSound);
                network.sendMessage("10 " + std::to_string(id) + "\n");
                break;
        }
    }
}

void Tank::move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze& maze, Health& health, Network& network, int my_id, int id, Mix_Chunk *gHealthPickSound) {
    int prevX = mPosX, prevY = mPosY;
    delay++;
    delay_time++;
    if (delay_time>30){                  // tune this
        delay_time=0;
        if (check_delay>2) check_delay--;   // tune this
    }
    if (delay<check_delay) return;
    delay=0;
    for(int i = 0; i < std::abs(mVelX); i++) {
        if(mVelX > 0) mPosX++;
        else mPosX--;
        if(mPosX < 0 || mPosX + TANK_WIDTH > SCREEN_WIDTH || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX, mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH, mPosY+TANK_HEIGHT)) {
            if(mVelX > 0) mPosX--;
            else mPosX++;
            break;
        }
        if(id == my_id && health.hasHealth(mPosX+TANK_WIDTH/2, mPosY+TANK_HEIGHT/2)) {
            Mix_PlayChannel( -1, gHealthPickSound, 0 );
            std::string message = "7 " + std::to_string(my_id) + " " + std::to_string(mPosX+TANK_WIDTH/2) + " " + std::to_string(mPosY+TANK_HEIGHT/2) + "\n";
            network.sendMessage(message);
        }
    }
    for(int i = 0; i < std::abs(mVelY); i++) {
        if(mVelY > 0) mPosY++;
        else mPosY--;
        if(mPosY < 0 || mPosY + TANK_HEIGHT > SCREEN_HEIGHT || maze.check_wall(mPosX, mPosY) || maze.check_wall(mPosX+TANK_WIDTH, mPosY) || maze.check_wall(mPosX, mPosY+TANK_HEIGHT) || maze.check_wall(mPosX+TANK_WIDTH, mPosY+TANK_HEIGHT)) {
            if(mVelY > 0) mPosY--;
            else mPosY++;
            break;
        }
        if(id == my_id && health.hasHealth(mPosX+TANK_WIDTH/2, mPosY+TANK_HEIGHT/2)) {
            Mix_PlayChannel( -1, gHealthPickSound, 0 );
            std::string message = "7 " + std::to_string(my_id) + " " + std::to_string(mPosX+TANK_WIDTH/2) + " " + std::to_string(mPosY+TANK_HEIGHT/2) + "\n";
            network.sendMessage(message);
        }
    }

    if(prevX != mPosX || prevY != mPosY) {
        network.sendMessage("3 " + std::to_string(id) + " " + std::to_string(mPosX) + " " + std::to_string(mPosY) + " " + std::to_string(face) + "\n");
    }
}

void Tank::render(SDL_Renderer* renderer, Texture &mTankTexture, Texture &mBulletTexture) {
    if(face == up) degree = 0;
    else if(face == right) degree = 90;
    else if(face == down) degree = 180;
    else degree = 270;
	mTankTexture.render(renderer, mPosX, mPosY, NULL, degree);
}

void Tank::fire(std::vector<Bullet*> &bullets, int id, Mix_Chunk *gBulletSound) {
    Mix_PlayChannel( -1, gBulletSound, 0 );
    Bullet* bullet=new Bullet(mPosX+5,mPosY+5,face, id);  
    bullets.push_back(bullet);
}

void Tank::moveTo(int x, int y) {
    mPosX = x;
    mPosY = y;
}

void Tank::setFace(int f) {
    face = f;
}