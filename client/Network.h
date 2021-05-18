#ifndef NETWORK_H
#define NETWORK_H
#pragma once
#include "Main.h"

class Network {
public:
    Network();
    ~Network();

    void connectToServer();
    bool incomingMessage();
    std::vector<std::string> getMessage();
    void sendMessage(std::string message);
    void handleEvent(SDL_Event& e, int my_id);
    void close();

private:
    char requestData[1400];
    SDLNet_SocketSet server;
    TCPsocket connection;
};

#endif