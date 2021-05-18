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
	if(code == 5 || code == 6) {
		int id = (message[2] - '0');
		network.removePlayer(id);
		players--;
	}
	network.sendAll(toSend);
}

int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

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

			if(players == 0) {
				int id = network.getWinner();
				network.sendAll("7 " + (id+'0') + '\n');
				break;
			}
		}
		if(players == 0) break;
	}

	network.sendAll("-1\n");

	network.close();
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}