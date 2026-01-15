#include "ServerApp.h"
#include "ChatRoutes.h"
#include "GameRoutes.h"
#include "LobbyRoutes.h"
#include "AuthRoutes.h"
#include <format>

ServerApp::ServerApp()
{
    setupRoutes();
}

void ServerApp::setupRoutes()
{
    registerChatRoutes(m_app, m_chat, m_gameManager);
    registerGameRoutes(m_app, m_gameManager);
    registerLobbyRoutes(m_app, m_gameManager);
    registerAuthRoutes(m_app, m_gameManager);
}

void ServerApp::run()
{
    std::cout << std::format("[Server] Starting on port {}... \n", SERVER_PORT);

    m_app.port(SERVER_PORT).multithreaded().run();
}
