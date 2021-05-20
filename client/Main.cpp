#include "Main.h"

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture gTextTexture;
Texture gInputTextTexture;
Texture gTankTexture[4];
Texture gBulletTexture;
Texture gHeartTexture;
Texture gPlayerTexture[4];
Texture gNotifTexture;
//The music that will be played
Mix_Music *gGameMusic = NULL;  
//The sound effects that will be used
Mix_Chunk *gBulletSound = NULL;
Mix_Chunk *gChangeTabSound = NULL;
Mix_Chunk *gHealthPickSound = NULL;
Mix_Chunk *gPlayerLostSound = NULL;
Mix_Chunk *gWinnerSound = NULL;

Maze maze;
Player* players[4];
Health health;
Network network;
Timer timer,timer1;
std::vector<Bullet*> bullets;
std::map<int,std::string> idToName;

std::string playerName = "";
bool quit = false;
bool start = false;
bool connect = false;
bool alive = true;
int tot_players = -1;
int my_id = -1;
int winning_id=-1;
bool background_sound=true;

void processMessage(std::vector<std::string> messages) {
	for(std::string message : messages) {
		std::cout << "Recieved: " << message << "\n";
		int code = message[0] - '0';
		if(code == 0) {
			my_id = message[2] - '0';
			tot_players = message[4] - '0';
			int seed = message[6] - '0';
			maze.generate(seed);
			network.sendMessage("8 " + std::to_string(my_id) + " " + playerName + "\n");
		} else if(code == 1 && message.size() < 3) {
			start = true;
			timer.start();
		} else if(code == 2) {
			int point = 2;
			int x = 0;
			int y = 0;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				x *= 10;
				x += (message[point]-'0');
				point++;
			}
			point++;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				y *= 10;
				y += (message[point]-'0');
				point++;
			}
			health.createHealth(x, y);
		} else if(code == 3) {
			int id = message[2] - '0';
			int x = 0, y = 0;
			int point = 4;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				x *= 10;
				x += (message[point]-'0');
				point++;
			}
			point++;
			while((message[point]-'0') >= 0 && (message[point]-'0') <= 9) {
				y *= 10;
				y += (message[point]-'0');
				point++;
			}
			point++;
			int f = message[point] - '0';
			if(id != my_id) {
				players[id]->moveTo(x, y);
				players[id]->setFace(f);
			}
		} else if(code == 4) {
			int id = message[2] - '0';
			players[id]->setHealth(0);
			std::stringstream notifText;
			notifText.str("");
			notifText << idToName[id] << " disconnected";
			SDL_Color textColor = {0,0,255};
			gNotifTexture.loadFromRenderedText(gRenderer, notifText.str().c_str(), textColor);
		} else if(code == 5) {
			int id = message[2] - '0';
			players[id]->setHealth(0);
			std::stringstream notifText;
			notifText.str("");
			notifText << idToName[id] << " died";
			SDL_Color textColor = {0,0,255};
			gNotifTexture.loadFromRenderedText(gRenderer, notifText.str().c_str(), textColor);
		} else if(code == 6) {
			int id = -1;
			for(int i = 0; i < tot_players; i++) {
				if(!players[i]->isDead()) id = i;
			}
			if (winning_id==-1) winning_id=id;
			std::stringstream notifText;
			notifText.str("");
			notifText << idToName[id] << " won";
			SDL_Color textColor = {0,0,255};
			gNotifTexture.loadFromRenderedText(gRenderer, notifText.str().c_str(), textColor);
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
		} else if(code == 8) {
			int id = message[2] - '0';
			std::string name = "";
			int point = 4;
			while(point < message.size()) {
				name += message[point];
				point++;
			}
			idToName[id] = name;
		} else if(code == 9) {
			int id = message[2] - '0';
			int f = message[4] - '0';
			if(id != my_id) players[id]->setFace(f);
		} else if(code == 1 && message.size() >= 3) {
			int id = message[3] - '0';
			if(id != my_id) players[id]->fire(bullets, gBulletSound);
		}
	}
}

void moveBullets() {
	int tot_bullets=bullets.size();
    int counter=0;
    while(counter<tot_bullets){
        bullets[counter]->move(SCREEN_WIDTH, SCREEN_HEIGHT, maze);
        bool active=bullets[counter]->active;
        if (active) counter++;
        else{
            auto it=bullets.begin()+counter;
            bullets.erase(it);
            tot_bullets--;
        }
    }
}

bool checkCollision(int posX, int posY, int id) {
	int counter = 0;
	for(Bullet* b : bullets) {
		bool hit = true;
		int X = b->mPosX, Y = b->mPosY;
		if(X > posX+Tank::TANK_WIDTH || posX > X+Bullet::BULLET_WIDTH) return false;
		if(Y > posY+Tank::TANK_HEIGHT || posY > Y+Bullet::BULLET_HEIGHT) return false;
		if(b->id == id) return false;
		if(hit) {
			b->active = false;
			return true;
		}
	}
	return false;
}

void renderBullets() {
	int tot_bullets=bullets.size();
    for(int i=0;i<tot_bullets;i++){
        bullets[i]->render(gRenderer, gBulletTexture);
    }
}

bool init() {
	bool success = true;
	idToName[0] = "Player 0";
	idToName[1] = "Player 1";
	idToName[2] = "Player 2";
	idToName[3] = "Player 3";

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
		//Initialize SDL_mixer
		if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
			success = false;
		}
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("LaserTank", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	if(!gTextTexture.loadFromRenderedText(gRenderer, "Enter you NAME then Press ENTER to connect to server:", textColor)) {
		printf( "Failed to render text texture!\n" );
		success = false;
	}
	if(!gTankTexture[0].loadFromFile(gRenderer, "images/tank_red.bmp")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gTankTexture[1].loadFromFile(gRenderer, "images/tank_cyan.png")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gTankTexture[2].loadFromFile(gRenderer, "images/tank_yellow.png")) {
		printf( "Failed to load tank texture!\n" );
		success = false;
	}
	if(!gTankTexture[3].loadFromFile(gRenderer, "images/tank_purple.png")) {
		printf( "Failed to render text texture!\n" );
		success = false;
	}
	textColor = {0, 0, 255};
	if(!gNotifTexture.loadFromRenderedText(gRenderer, "Game Started!", textColor)) {
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
	//Load music
	gGameMusic = Mix_LoadMUS( "sounds/game_music_low.wav" );
	if( gGameMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gBulletSound = Mix_LoadWAV( "sounds/wave_sound.wav" );
	if( gBulletSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gChangeTabSound = Mix_LoadWAV( "sounds/change_tab.wav" );
	if( gChangeTabSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gHealthPickSound = Mix_LoadWAV( "sounds/health_pick.wav" );
	Mix_VolumeChunk(gHealthPickSound,30);
	if( gHealthPickSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gPlayerLostSound = Mix_LoadWAV( "sounds/lost.wav" );
	if( gPlayerLostSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gWinnerSound = Mix_LoadWAV( "sounds/winner_music2.wav" );
	if( gWinnerSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void close() {
	
	gTextTexture.free();
	gInputTextTexture.free();
	gHeartTexture.free();
	gNotifTexture.free();
	Mix_FreeChunk( gBulletSound );
	gBulletSound = NULL;
	Mix_FreeChunk( gChangeTabSound );
	gChangeTabSound = NULL;
	Mix_FreeChunk( gHealthPickSound );
	gHealthPickSound = NULL;
	Mix_FreeChunk( gPlayerLostSound );
	gPlayerLostSound = NULL;
	Mix_FreeChunk( gWinnerSound );
	gWinnerSound = NULL;
	//Free the music
	Mix_FreeMusic( gGameMusic );
	gGameMusic = NULL;

	for(int i = 0; i < 4; i++) gTankTexture[i].free(),gPlayerTexture[i].free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
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

			SDL_Color textColor = {255, 64, 0};

			timer1.start();

			//Enable text input
			SDL_StartTextInput();

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderClear(gRenderer);

			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						if(alive) network.sendMessage("4 " + std::to_string(my_id)+"\n");
						quit = true;
					} else if(e.type == SDL_KEYDOWN) {
						if(e.key.keysym.sym == SDLK_RETURN) {
							Mix_PlayChannel( -1, gChangeTabSound, 0 );
							if(network.connectToServer(gRenderer, gTextTexture)) {
								connect = true;
								if(!gTextTexture.loadFromRenderedText(gRenderer, "Waiting for all players to join ...", textColor)) {
									printf("Failed to render text texture!\n");
									exit(1);
								}
							}
							SDL_StopTextInput();
						}else if ( e.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0 )
						{
							//lop off character
							playerName.pop_back();
						}
					}//Special text input event
					else if( e.type == SDL_TEXTINPUT )
					{
						//Append character
						playerName += e.text.text;
					}
					if(start && alive) players[my_id]->handleEvent(e, gBulletSound, network, bullets);
					if(connect && network.incomingMessage()) processMessage(network.getMessage());
				}

				if(connect && network.incomingMessage()) processMessage(network.getMessage());

				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderClear(gRenderer);

				if(start) {
					if(timer.getTicks() <= 2000) maze.render(gRenderer, 255*timer.getTicks()/2000);
					else {
						if( Mix_PlayingMusic() == 0 )
						{
							//Play the music
							if (background_sound) Mix_PlayMusic( gGameMusic, -1 );
						}
						if (winning_id==my_id){
							winning_id=-2;
							Mix_PlayChannel( -1, gWinnerSound, 0 );
						}
						maze.render(gRenderer, 255);
						gNotifTexture.render(gRenderer, (SCREEN_WIDTH - gNotifTexture.getWidth())/2, BY);
					}
				} else {
					//Text is not empty
					if( playerName != "" )
					{
						std::string temp=playerName;
						if (timer1.getTicks()>300){
							temp+='|';
							if (timer1.getTicks()>800)  {
								timer1.start();
							}
						}
						//Render new text
						gInputTextTexture.loadFromRenderedText(gRenderer, temp.c_str(), textColor );
					}
					//Text is empty
					else
					{
						//Render space texture
						std::string temp=" ";
						if (timer1.getTicks()>300){
							temp+='|';
							if (timer1.getTicks()>800)  {
								timer1.start();
							}
						}
						gInputTextTexture.loadFromRenderedText(gRenderer, temp.c_str(), textColor );
					}
					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth())/2, (SCREEN_HEIGHT - gTextTexture.getHeight())/2);
					if (!connect) gInputTextTexture.render(gRenderer, ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, (SCREEN_HEIGHT + gTextTexture.getHeight())/2 );
				}	

				if(start && timer.getTicks() > 2500) {
					for(int i = 0; i < tot_players; i++) {
						if(players[i]->isDead()) continue;
						players[i]->move(maze, health, network, my_id, gHealthPickSound);
						players[i]->render(gRenderer, gPlayerTexture, gTankTexture, gBulletTexture, idToName);
					}
					moveBullets();
					renderBullets();
				}

				if(start) {
					for(int i = 0; i < tot_players; i++) {
						if(players[i]->isDead()) continue;
						if(checkCollision(players[i]->getX(), players[i]->getY(), i)) {
							players[i]->bulletHit();
						}
					}
				}

				if(start && alive && players[my_id]->isDead()) {
					network.sendMessage("5 " + std::to_string(my_id) + "\n");
					alive = false;
					Mix_PlayChannel( -1, gPlayerLostSound, 0 );
				}

				for(int i = 0; i < tot_players; i++) {
					if(start && players[i]->isDead()) continue;
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