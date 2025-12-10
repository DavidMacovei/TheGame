#pragma once
#include "GameModels.h"


class ClientApi
{
public:
    explicit ClientApi(const std::string& baseUrl = "http://127.0.0.1:18080");

    BasicResponse login(const std::string& username, const std::string& password);
    BasicResponse registerUser(const std::string& username, const std::string& password);

    BasicResponse joinLobby(const std::string& username);
    LobbyState getLobbyState();

    GameState getGameState(int myPlayerIndex);
    BasicResponse playCard(int playerIndex, int handIndex, int stackIndex);
    BasicResponse endTurn(int playerIndex);

    BasicResponse sendMessage(const std::string& sender, const std::string& message);
    ChatHistory getChatHistory();
    
private:
    std::string baseUrl;
};

