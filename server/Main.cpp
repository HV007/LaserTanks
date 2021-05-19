#include "Main.h"

bool init();
bool loadMedia();
void close();

Network network;

int players;
int tot_players;
int seed;

void processString(std::string message) {
	std::cout << "Recieved: " << message;
	int code = message[0] - '0';
	std::string toSend = message;
	if(code == 4) {
		int id = (message[2] - '0');
		network.removePlayer(id);
		players--;
	} else if(code == 5) {
		players--;
	}
	network.sendAll(toSend);
}

int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	srand(time(0));

	players = 0;
	tot_players = 0;
	seed = rand()%100;

	while(true) {
		std::cout << "Enter number of players: ";
		std::cin >> tot_players;
		if(tot_players > 0 && tot_players <= MAX_PLAYERS) break;
		else std::cout << "Max " << MAX_PLAYERS << " allowed\n";
	}

	while(players < tot_players) {
		if(network.acceptConnection(tot_players, seed)) players++;
	}

	network.sendAll("1\n");

	bool running = true;

	while(running) {
		while(network.incomingMessage()) {
			std::string message = network.getMessage();
			processString(message);

			if(tot_players > 1 && players == 1) {
				network.sendAll("6 \n");
			}

			if(players == 0) {
				break;
			}
		}
		if(players == 0) break;
	}

	network.sendAll("-1\n");
	network.close();
	SDLNet_Quit();
	SDL_Quit();
}