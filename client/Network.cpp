#include "Network.h"

std::vector<std::string> convertToString(char* a, int size) {
    std::vector<std::string> res;
    std::string s = "";
    for (int i = 0; i < size; i++) {
        if(a[i] == '\0') break;
        if(a[i] == '\n') {
            res.push_back(s);
            s = "";
        } else s = s + a[i];
    }
    return res;
}

Network::Network() {
    SDLNet_Init();
}

Network::~Network() {
    close();
}

bool Network::connectToServer(SDL_Renderer *gRenderer, Texture &gTextTexture) {
    IPaddress ip;
    if(SDLNet_ResolveHost(&ip, IP_ADDRESS.c_str(), 3000) == -1) {
        SDL_Color textColor = {255, 64, 0};
        gTextTexture.loadFromRenderedText(gRenderer, "Error connecting to server", textColor);
        std::cout << "Error connecting to server\n";
        return false;
    }
    connection = SDLNet_TCP_Open(&ip);
    if(connection == NULL) {
        SDL_Color textColor = {255, 64, 0};
        gTextTexture.loadFromRenderedText(gRenderer, "Error connecting to server", textColor);
        std::cout << "Error connecting to server\n";
        return false;
    }
    server = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(server, connection);
    return true;
}

void Network::sendMessage(std::string message) {
    strcpy(requestData, message.c_str());
    SDLNet_TCP_Send(connection, requestData, strlen(requestData)+1);
}

bool Network::incomingMessage() {
    int a = SDLNet_CheckSockets(server, 0);
    return a > 0;
}

std::vector<std::string> Network::getMessage() {
    memset(requestData, 0, 1400);
    SDLNet_TCP_Recv(connection, requestData, 1400);
    std::vector<std::string> message =  convertToString(requestData, 1400);
    return message;
}

void Network::handleEvent(SDL_Event& e, int my_id) {
    int a = -1, b = -1, c = -1;
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        a = 1; b = 0;
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT: c = 4; break;
            case SDLK_UP: c = 1; break;
            case SDLK_DOWN: c = 3; break;
            case SDLK_LEFT: c = 0; break;
            case SDLK_RIGHT: c = 2; break;
            case SDLK_SPACE: c = 5; break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
        a = 0; b = 0;
        switch(e.key.keysym.sym) {
            case SDLK_LSHIFT:
                c = 4;
                break;
            case SDLK_SPACE:
                c = 5;
                break;
        }
    }
    
    if(c != -1) {
        std::string message = "3 " + std::to_string(my_id) + " " + std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + "\n";
        sendMessage(message);
    }
}

void Network::close() {
    SDLNet_TCP_Close(connection);
    SDLNet_FreeSocketSet(server); 
    SDLNet_Quit();
}