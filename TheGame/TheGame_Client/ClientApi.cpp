#include "ClientApi.h"
#include <cpr/cpr.h>
#include <iostream>

ClientApi::ClientApi(const std::string& baseUrl) : baseUrl(baseUrl) {}

BasicResponse ClientApi::login(const std::string& username, const std::string& password)
{
    try {
        AuthRequest req;
        req.username = username;
        req.password = password;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/login" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty()) 
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in login: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::registerUser(const std::string& username, const std::string& password)
{
    try {
        AuthRequest req;
        req.username = username;
        req.password = password;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/register" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty())
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in registerUser: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::joinLobby(const std::string& username)
{
    try {
        JoinLobbyRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/joinLobby" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty())
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in joinLobby: " + std::string(e.what()) };
    }
}

LobbyState ClientApi::getLobbyState()
{
    try {
        auto r = cpr::Get(cpr::Url{ baseUrl + "/lobbyState" });

        if (r.status_code == 200)
            return json::parse(r.text).get<LobbyState>();
    }
    catch (...) {}

    LobbyState errState;
    errState.status = "Connection Error";
    return errState;
}

GameState ClientApi::getGameState(int myPlayerIndex)
{
    try {
        auto r = cpr::Get(cpr::Url{ baseUrl + "/gameState/" + std::to_string(myPlayerIndex)});

        if (r.status_code == 200)
            return json::parse(r.text).get<GameState>();
    }
    catch (...) {}

    GameState errState;
    errState.status = "Connection Error";
    return errState;
}

BasicResponse ClientApi::playCard(int playerIndex, int handIndex, int stackIndex)
{
    try {
        PlayCardAction action;
        action.playerIndex = playerIndex;
        action.handIndex = handIndex;
        action.stackIndex = stackIndex;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/playCard" },
            cpr::Body{ json(action).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty())
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in playCard: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::endTurn(int playerIndex)
{
    try {
        EndTurnAction action;
        action.playerIndex = playerIndex;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/endTurn" },
            cpr::Body{ json(action).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty())
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in endTurn: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::sendMessage(const std::string& sender, const std::string& message)
{
    try {
        ChatMessageRequest req;
        req.sender = sender;
        req.message = message;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/sendMessage" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty())
            return { "error", "Server not responding" };

        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Exception in sendMessage: " + std::string(e.what()) };
    }
}

ChatHistory ClientApi::getChatHistory()
{
    try {
        auto r = cpr::Get(cpr::Url{ baseUrl + "/getMessages" });

        if (r.status_code == 200)
            return json::parse(r.text).get<ChatHistory>();
    }
    catch (...) {}

    return ChatHistory();
}





