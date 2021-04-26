#ifndef DOT_H
#define DOT_H
#include "Main.h"

class Texture;

class Tank {
    public:
		static const int TANK_WIDTH = 15;
		static const int TANK_HEIGHT = 15;

		static const int TANK_VEL = 10;

		Tank();

		void handleEvent(SDL_Event& e);
		void move(int SCREEN_WIDTH, int SCREEN_HEIGHT);
		void render(SDL_Renderer* renderer, Texture &mTankTexture);

    private:
		int mPosX, mPosY;
		int mVelX, mVelY;
};

#endif