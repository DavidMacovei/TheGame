#include "ServerApp.h"

#include "ChatRoutes.h"
#include "GameRoutes.h"
#include "LobbyRoutes.h"
#include "AuthRoutes.h"

ServerApp::ServerApp()
{
    setupRoutes();
}

void ServerApp::setupRoutes()
{
    registerChatRoutes(app, chat);
    registerGameRoutes(app, activeGame);
    registerLobbyRoutes(app, activeGame, lobbyPlayers);
    registerAuthRoutes(app);
}

void ServerApp::run()
{
    app.port(18080).multithreaded().run();
}
