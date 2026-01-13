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
    crow::SimpleApp app; //referinta

    ChatService chat;

    game::GameManager gameManager;
};