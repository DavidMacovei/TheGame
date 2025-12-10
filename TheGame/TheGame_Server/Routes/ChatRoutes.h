#pragma once
#include <crow.h>
#include "ChatService.h"

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat);
