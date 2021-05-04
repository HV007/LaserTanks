#include "Player.h"

Player::Player(int id): id(id), nextTick(3500) {
    health = 100;
    tank = new Tank();
}

void Player::handleEvent(SDL_Event &e) {
    tank->handleEvent(e);
}

void Player::move(Maze& maze, Health& h) {
    tank->move(SCREEN_WIDTH, SCREEN_HEIGHT, maze, h, health);
}

void Player::reduceHealth() {
    nextTick += 1000;
    if(health > 0) health--;
}

int Player::getHealth() {
    return health;
}

int Player::getNextTick() {
    return nextTick;
}

void Player::render(SDL_Renderer *renderer, Texture mPlayerTexture[], Texture& mTankTexture) {
    tank->render(renderer, mTankTexture);
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