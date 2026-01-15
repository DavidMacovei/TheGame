#pragma once

#include <crow.h>
#include <memory>
#include <vector>
#include <mutex>

#include "ChatService.h"
#include "GameManager.h"

class ServerApp
{
public:
    ServerApp();
    void run();

private:
    void setupRoutes();

private:
    ChatService m_chat;
    game::GameManager m_gameManager;
    crow::SimpleApp m_app;

    static const uint16_t SERVER_PORT = 18080;
};