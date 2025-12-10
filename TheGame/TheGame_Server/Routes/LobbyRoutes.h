#pragma once
#include <crow.h>
#include <memory>
#include <vector>
#include "Game.h"

void registerLobbyRoutes(
    crow::SimpleApp& app,
    std::unique_ptr<game::Game>& activeGame,
    std::vector<std::string>& lobbyPlayers
);