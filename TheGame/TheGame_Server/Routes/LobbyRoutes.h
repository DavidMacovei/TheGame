#pragma once
#include <crow.h>
#include <memory>
#include <vector>
#include <mutex>
#include "GameManager.h"

void registerLobbyRoutes(
    crow::SimpleApp& app,
    game::GameManager& gameManager
);