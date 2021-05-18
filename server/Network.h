#ifndef NETWORK_H
#define NETWORK_H
#pragma once
#include "Main.h"

struct data {
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket socket, Uint32 timeout, int id): socket(socket), timeout(timeout), id(id) {};
};

class Network {
public:
    Network();
    ~Network();

    bool acceptConnection(int tot_players, int seed);
    void sendAll(std::string meesage);
    bool incomingMessage();
    std::string getMessage();
    void removePlayer(int id);
    int getWinner();
    void close();

private:
    int currid;
    IPaddress ip;
    std::vector<data> socketData;
    char requestData[1400];
    SDLNet_SocketSet sockets;
    TCPsocket server;
};


#endif