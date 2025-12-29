#pragma once

#include <crow.h>
#include <memory>
#include <vector>
#include <mutex>

#include "ChatService.h"
#include "Game.h"

class ServerApp
{
public:
    ServerApp();
    void run();

private:
    crow::SimpleApp app;

    ChatService chat;

    std::unique_ptr<game::Game> activeGame;
    std::vector<std::string> lobbyPlayers;

    std::mutex gameMutex;

    void setupRoutes();
};