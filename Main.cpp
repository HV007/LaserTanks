#include "Main.h"

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture gTextTexture;
Texture gTankTexture;
Texture gHeartTexture;
Texture gPlayerTexture[4];

Maze maze;
// Tank tank;
Player player(0);
Health health;

bool init() {
	srand(time(0));
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if(TTF_Init() == -1) {
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
			return false;
		}
		int imgFlags = IMG_INIT_PNG;
		if(!(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
			return false;
		}
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				maze.initialise();
				health.initialise();
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	SDL_Color textColor = {255, 64, 0};
	if(!gTextTexture.loadFromRenderedText(gRenderer, "Press ENTER to start:", textColor)) {
		printf( "Failed to render text texture!\n" );
		success = false;
	}
	if(!gTankTexture.loadFromFile(gRenderer, "images/tank1.png")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gHeartTexture.loadFromFile(gRenderer, "images/heart.png")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}

    maze.generate();

	return success;
}

void close() {
	gTankTexture.free();
	gTextTexture.free();
	gHeartTexture.free();

	for(int i = 0; i < 4; i++) gPlayerTexture[i].free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if(!init()) {
		printf( "Failed to initialize!\n" );
	} else {
		if(!loadMedia()) {
			printf( "Failed to load media!\n" );
		} else {	
			bool quit = false;
			bool start = false;

			SDL_Event e;

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderClear(gRenderer);

			Timer timer;

			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						quit = true;
					} else if(e.type == SDL_KEYDOWN) {
						if(e.key.keysym.sym == SDLK_RETURN) {
							start = true;
							timer.start();
						}
					}
					player.handleEvent(e);
					// tank.handleEvent(e);
				}
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderClear(gRenderer);

				if(start) {
					if(timer.getTicks() <= 2000) maze.render(gRenderer, 255*timer.getTicks()/2000);
					else maze.render(gRenderer, 255);
				} else gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth())/2, (SCREEN_HEIGHT - gTextTexture.getHeight())/2);

				if(start && timer.getTicks() > 2500) {
					player.move(maze, health);
					player.render(gRenderer, gPlayerTexture, gTankTexture);
					// tank.move(SCREEN_WIDTH, SCREEN_HEIGHT, maze);
					// tank.render(gRenderer, gTankTexture);
				}

				if(start && timer.getTicks() >= player.getNextTick()) {
					player.reduceHealth();
				}

				if(start && timer.getTicks() >= health.getNextTick()) {
					health.generate();
				}

				if(start) health.render(gRenderer, gHeartTexture);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();

	return 0;
}