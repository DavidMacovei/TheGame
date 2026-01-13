#pragma once
#include <crow.h>

namespace game { class GameManager; }

void registerAuthRoutes(crow::SimpleApp& app, game::GameManager& gameManager);
