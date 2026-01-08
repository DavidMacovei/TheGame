#pragma once
#include <crow.h>
#include "ChatService.h"
#include "GameManager.h"

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat, game::GameManager& gameManager);
