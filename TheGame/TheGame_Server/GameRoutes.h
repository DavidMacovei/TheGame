#pragma once
#include <crow.h>
#include <memory>
#include "Game.h"

void registerGameRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame);