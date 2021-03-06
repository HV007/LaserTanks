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

		Tank(int x, int y, int f);

		void render(SDL_Renderer* renderer, Texture &mTankTexture, Texture &mBulletTexture);
		void fire(std::vector<Bullet*> &bullets, int id, Mix_Chunk *gBulletSound);
		void handleEvent(SDL_Event& e, int id, Mix_Chunk *gBulletSound, Network& network, std::vector<Bullet*> &bullets);
		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT, Maze& maze, Health& health, Network& network, int my_id, int id, Mix_Chunk *gHealthPickSound);
		int mPosX, mPosY;
		void moveTo(int x, int y);
		void setFace(int f);

    private:
		int mVelX, mVelY;
		int face,degree;
		int delay;
		int check_delay,delay_time;
};

#endif