#include "Main.h"

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture gTextTexture;
Texture gTankTexture;
Texture gBulletTexture;
Texture gHeartTexture;
Texture gPlayerTexture[4];

Maze maze;
Player* players[4];
Health health;
Network network;
Timer timer;

bool quit = false;
bool start = false;
bool connect = false;
int tot_players = -1;
int my_id = -1;

void processMessage(std::vector<std::string> messages) {
	for(std::string message : messages) {
		std::cout << "Recieved: " << message << "\n";
		int code = message[0] - '0';
		if(code == 0) {
			my_id = message[2] - '0';
			tot_players = message[4] - '0';
			int seed = message[6] - '0';
			maze.generate(seed);
		} else if(code == 1) {
			start = true;
			timer.start();
		} else if(code == 2) {
			int x = message[2] - '0'; // not correct
			int y = message[4] - '0';
			health.createHealth(x, y);
		} else if(code == 3) {
			int id = message[2] - '0';
			int a = message[4] - '0';
			int b = message[6] - '0';
			int c = message[8] - '0';
			players[id]->handleEvent(a, b, c);
		} else if(code == 7) {
			int id = message[2] - '0';
			int point = 4;
			int x = 0;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				x *= 10;
				x += (message[point]-'0');
				point++;
			}
			point++;
			int y = 0;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				y *= 10;
				y += (message[point]-'0');
				point++;
			}
			health.collectHealth(x, y);
			players[id]->increaseHealth();
		}
	}
}

bool init() {
	bool success = true;

	for(int i = 0; i < 4; i++) {
		players[i] = new Player(i);
	}

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
	if(!gTextTexture.loadFromRenderedText(gRenderer, "Press ENTER to connect to server:", textColor)) {
		printf( "Failed to render text texture!\n" );
		success = false;
	}
	if(!gTankTexture.loadFromFile(gRenderer, "images/tank1.bmp")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gBulletTexture.loadFromFile(gRenderer, "images/wifi.bmp")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gHeartTexture.loadFromFile(gRenderer, "images/heart.png")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}

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

			SDL_Event e;

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderClear(gRenderer);

			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						quit = true;
					} else if(e.type == SDL_KEYDOWN) {
						if(e.key.keysym.sym == SDLK_RETURN) {
							network.connectToServer();
							connect = true;
							SDL_Color textColor = {255, 64, 0};
							if(!gTextTexture.loadFromRenderedText(gRenderer, "Waiting for all players to join ...", textColor)) {
								printf("Failed to render text texture!\n");
								exit(1);
							}
						}
					}
					if(start) network.handleEvent(e, my_id);
					if(connect && network.incomingMessage()) processMessage(network.getMessage());
				}

				if(connect && network.incomingMessage()) processMessage(network.getMessage());

				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderClear(gRenderer);

				if(start) {
					if(timer.getTicks() <= 2000) maze.render(gRenderer, 255*timer.getTicks()/2000);
					else maze.render(gRenderer, 255);
				} else gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth())/2, (SCREEN_HEIGHT - gTextTexture.getHeight())/2);

				if(start && timer.getTicks() > 2500) {
					for(int i = 0; i < tot_players; i++) {
						players[i]->move(maze, health, network, my_id);
						players[i]->render(gRenderer, gPlayerTexture, gTankTexture);
					}
				}

				for(int i = 0; i < tot_players; i++) {
					if(start && timer.getTicks() >= players[i]->getNextTick()) players[i]->reduceHealth();
				}

				if(start && timer.getTicks() >= health.getNextTick()) {
					if(my_id == 0) {
						std::pair<int,int> p =  health.generate();
						std::string message = "2 " + std::to_string(p.first) + " " + std::to_string(p.second) + "\n";
						network.sendMessage(message);
					}
				}

				if(start) health.render(gRenderer, gHeartTexture);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	close();

	return 0;
}