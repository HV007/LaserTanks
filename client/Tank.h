#ifndef DOT_H
#define DOT_H
#include "Main.h"

class Texture;
class Maze;
class Health;
class Bullet;
class Network;

class Tank {
    public:
		static const int TANK_WIDTH = 24;
		static const int TANK_HEIGHT = 24;

		static const int TANK_VEL = 1;

		Tank();

		void render(SDL_Renderer* renderer, Texture &mTankTexture, Texture &mBulletTexture);
		void fire(std::vector<Bullet*> &bullets, int id);
		void handleEvent(int a, int b, int c, std::vector<Bullet*> &bullets, int id);
		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze& maze, Health& health, Network& network, int my_id);
		int mPosX, mPosY;

    private:
		int mVelX, mVelY;
		int face,degree;
		int delay;
};

#endif