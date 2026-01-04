#pragma once
#include "GameModels.h"


class ClientApi
{
public:
    explicit ClientApi(const std::string& baseUrl = "http://127.0.0.1:18080");

    BasicResponse Login(const std::string& username, const std::string& password);
    BasicResponse RegisterUser(const std::string& username, const std::string& password);

    JoinGameResponse JoinLobby(const std::string& username);
    LobbyState GetLobbyState();

    GameState GetGameState(int myPlayerIndex);
    BasicResponse PlayCard(int playerIndex, int handIndex, int stackIndex);
    BasicResponse EndTurn(int playerIndex);

    BasicResponse sendMessage(const std::string& sender, const std::string& message);//SendMessage existenta?
    ChatHistory GetChatHistory();
    
private:
    std::string baseUrl;
};

