#ifndef BULLET_H
#define BULLET_H
#include "Main.h"

class Texture;
class Maze;

class Bullet {
    public:
		static const int BULLET_WIDTH = 16;
		static const int BULLET_HEIGHT = 16;

		static const int BULLET_VEL = 1;

		Bullet(int x,int y, int f);

		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze &maze);
		void render(SDL_Renderer* renderer, Texture &mBulletTexture);
		bool active;                          // check private or public
		int face,degree;
		int mPosX, mPosY;

    private:
		int mVelX, mVelY;
		
};

#endif