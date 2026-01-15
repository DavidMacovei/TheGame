#include "ServerApp.h"
#include "ChatRoutes.h"
#include "GameRoutes.h"
#include "LobbyRoutes.h"
#include "AuthRoutes.h"
#include "Logger.h"

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
    Logger::Info("Server starting on port {}...", SERVER_PORT);

    m_app.port(SERVER_PORT).multithreaded().run();
}
