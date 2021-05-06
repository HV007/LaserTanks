#ifndef DOT_H
#define DOT_H
#include "Main.h"

class Texture;
class Maze;
class Health;
class Bullet;

class Tank {
    public:
		static const int TANK_WIDTH = 24;
		static const int TANK_HEIGHT = 24;

		static const int TANK_VEL = 1;

		Tank();

		void handleEvent(SDL_Event& e);
		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze& maze, Health& health, int& h);
		void render(SDL_Renderer* renderer, Texture &mTankTexture, Texture &mBulletTexture);
		void fire();

    private:
		int mPosX, mPosY;
		int mVelX, mVelY;
		int face,degree;
		std::vector<Bullet *> bullets;
};

#endif