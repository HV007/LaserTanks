#ifndef BULLET_H
#define BULLET_H
#include "Main.h"

class Texture;
class Maze;

class Bullet {
    public:
		static const int BULLET_WIDTH = 15;
		static const int BULLET_HEIGHT = 15;

		static const int BULLET_VEL = 5;

		Tank();

		void handleEvent(SDL_Event& e);                    // may be do it in the tank class
		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze);
		void render(SDL_Renderer* renderer, Texture &mBulletTexture);

    private:
		int mPosX, mPosY;
		int mVelX, mVelY;
		int face,degree;
};

#endif