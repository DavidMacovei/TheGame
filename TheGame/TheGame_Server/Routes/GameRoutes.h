#pragma once
#include <crow.h>
#include <memory>
#include <mutex>
#include "Game.h"

void registerGameRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame, std::mutex& gameMutex);