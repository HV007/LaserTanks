#include "Player.h"

Player::Player(int id): id(id), nextTick(3500) {
    health = 100;
    tank = new Tank();
}

void Player::handleEvent(int a, int b, int c, std::vector<Bullet*> &bullets, Mix_Chunk *gBulletSound) {
    tank->handleEvent(a, b, c, bullets, id);
}

void Player::move(Maze& maze, Health& h, Network& network, int my_id, Mix_Chunk *gHealthPickSound) {
    tank->move(SCREEN_WIDTH, SCREEN_HEIGHT, maze, h, network, my_id, gHealthPickSound);
}

void Player::reduceHealth() {
    nextTick += 1000;
    if(health > 0) health--;
}

void Player::increaseHealth() {
    health = 100;
}

int Player::getHealth() {
    return health;
}

int Player::getNextTick() {
    return nextTick;
}

void Player::render(SDL_Renderer *renderer, Texture mPlayerTexture[], Texture& mTankTexture, Texture& mBulletTexture) {
    tank->render(renderer, mTankTexture, mBulletTexture);
    SDL_Color textColor;
    if(id == 0) {
        textColor = {255, 64, 0};
    } else if(id == 1) {
        textColor = {255, 64, 0};
    } else if(id == 2) {
        textColor = {255, 64, 0};
    } else {
        textColor = {255, 64, 0};
    }
    std::stringstream healthText;
    healthText.str("");
    healthText << "Player " << id+1 << ":- Health = " << health;
    if(!mPlayerTexture[id].loadFromRenderedText(renderer, healthText.str().c_str(), textColor)) {
		printf("Failed to render text texture!\n");
	}
    if(id == 0) {
        mPlayerTexture[id].render(renderer, BX, BY);
    } else if(id == 1) {
        mPlayerTexture[id].render(renderer, SCREEN_WIDTH - mPlayerTexture[id].getWidth() - BX, BY);
    } else if(id == 2) {
        mPlayerTexture[id].render(renderer, BX, SCREEN_HEIGHT - mPlayerTexture[id].getHeight() - BY);
    } else {
        mPlayerTexture[id].render(renderer, SCREEN_WIDTH - mPlayerTexture[id].getWidth() - BX, SCREEN_HEIGHT - mPlayerTexture[id].getHeight() - BY);
    }
}

int Player::getX() {
    return tank->mPosX;
}

int Player::getY() {
    return tank->mPosY;
}

void Player::bulletHit() {
    health -= 20;
}