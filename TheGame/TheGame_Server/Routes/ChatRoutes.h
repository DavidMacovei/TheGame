#pragma once

#include <crow.h>
#include "GameManager.h"

import chat;

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat, game::GameManager& gameManager);
