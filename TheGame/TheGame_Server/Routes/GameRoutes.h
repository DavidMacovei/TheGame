#pragma once
#include <crow.h>
#include <memory>
#include <mutex>
#include "GameManager.h"

void registerGameRoutes(crow::SimpleApp& app, game::GameManager& gameManager);