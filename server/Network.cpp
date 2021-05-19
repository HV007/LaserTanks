#include "Network.h"

std::string convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
        if(a[i] == '\n') break;
    }
    return s;
}

Network::Network() {
    if (SDLNet_ResolveHost(&ip, NULL, 3000) == -1) {
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(1);
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
      printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      exit(2);
    }
    sockets = SDLNet_AllocSocketSet(10);
    currid = 0;
}

Network::~Network() {
    close();
}

bool Network::acceptConnection(int tot_players, int seed) {
    TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
    if(tmpsocket) {
        SDLNet_TCP_AddSocket(sockets, tmpsocket);
        socketData.push_back(data(tmpsocket, SDL_GetTicks(), currid));
        std::cout << "Player " << currid << " connected\n";
        sprintf(requestData, "0 %d %d %d\n", currid, tot_players, seed);
        currid++;
        int result = SDLNet_TCP_Send(tmpsocket, requestData, strlen(requestData));
        return true;
    }
    return false;
}

void Network::sendAll(std::string message) {
    strcpy(requestData, message.c_str());
    for(int i = 0; i < socketData.size(); i++) {
        int result = SDLNet_TCP_Send(socketData[i].socket, requestData, strlen(requestData));
    }
}

bool Network::incomingMessage() {
    return SDLNet_CheckSockets(sockets, 0) > 0;
}

std::string Network::getMessage() {
    for(int i = 0; i < socketData.size(); i++) {
        if(SDLNet_SocketReady(socketData[i].socket)) {
            socketData[i].timeout = SDL_GetTicks();
            SDLNet_TCP_Recv(socketData[i].socket, requestData, 1400);
            return convertToString(requestData, 1400);
        }
    }
    return NULL;
}

void Network::removePlayer(int id) {
    for(int i = 0; i < socketData.size(); i++) {
        if(socketData[i].id == id) {
            SDLNet_TCP_DelSocket(sockets, socketData[i].socket);
            SDLNet_TCP_Close(socketData[i].socket);
            socketData.erase(socketData.begin() + i);
        }
    }
}

int Network::getWinner() {
    return socketData[0].id;
}

void Network::close() {
    
}