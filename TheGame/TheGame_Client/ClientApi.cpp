#include "ClientApi.h"
#include <iostream>

ClientApi::ClientApi(const std::string& baseUrl) : baseUrl(baseUrl) {}

BasicResponse ClientApi::Login(const std::string& username, const std::string& password)
{
    try {
        AuthRequest req;
        req.username = username;
        req.password = password;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/auth/login" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "Login");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in Login: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::RegisterUser(const std::string& username, const std::string& password)
{
    try {
        AuthRequest req;
        req.username = username;
        req.password = password;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/auth/register" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "Register");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in RegisterUser: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::JoinLobby(const std::string& username)
{
    try {
        UserRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/lobby/join" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "JoinLobby");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in JoinLobby: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::LeaveLobby(const std::string& username)
{
    try {
        UserRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/lobby/leave" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "LeaveLobby");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in LeaveLobby: " + std::string(e.what()) };
    }
}

UserStatusResponse ClientApi::GetUserStatus(const std::string& username)
{
    try {
        UserRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/lobby/status" },
            cpr::Body{json(req).dump()},
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.status_code == 200)
            return json::parse(r.text).get<UserStatusResponse>();
    }
    catch (...) {}

    UserStatusResponse err;
    err.status = "error";
    err.message = "Connection lost";
    return err;
}

GameState ClientApi::GetGameState(const std::string& username)
{
    if (m_activeGameId == -1)
        return {};

    try {
        UserRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/game/" + std::to_string(m_activeGameId) + "/state" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} });

        if (r.status_code == 200)
            return json::parse(r.text).get<GameState>();
    }
    catch (...) {}

    GameState err;
    err.status = "error";
    return err;
}

BasicResponse ClientApi::PlayCard(int playerIndex, int handIndex, int stackIndex)
{
    if (m_activeGameId == -1) 
        return { "error", "No active game" };

    try {
        PlayCardAction action;
        action.playerIndex = playerIndex;
        action.handIndex = handIndex;
        action.stackIndex = stackIndex;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/game/" + std::to_string(m_activeGameId) + "/playCard"},
            cpr::Body{ json(action).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "PlayCard");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in PlayCard: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::EndTurn(int playerIndex)
{
    if (m_activeGameId == -1) 
        return { "error", "No active game" };

    try {
        EndTurnAction action;
        action.playerIndex = playerIndex;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/game/" + std::to_string(m_activeGameId) + "/endTurn" },
            cpr::Body{ json(action).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "EndTurn");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in EndTurn: " + std::string(e.what()) };
    }
}

BasicResponse ClientApi::sendMessage(const std::string& sender, const std::string& message)
{
    if (m_activeGameId == -1) 
        return { "error", "No active game" };

    try {
        ChatMessageRequest req;
        req.sender = sender;
        req.message = message;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/game/" + std::to_string(m_activeGameId) + "/chat/send"},
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        return HandleResponse(r, "SendMessage");
    }
    catch (const std::exception& e) {
        return { "error", "Exception in SendMessage: " + std::string(e.what()) };
    }
}

ChatHistory ClientApi::GetChatHistory()
{
    if (m_activeGameId == -1)
        return {};

    try {
        auto r = cpr::Get(
            cpr::Url{ baseUrl + "/game/" + std::to_string(m_activeGameId) + "/chat/history" });

        if (r.status_code == 200)
            return json::parse(r.text).get<ChatHistory>();
    }
    catch (...) {}

    return {};
}

ProfileResponse ClientApi::GetProfile(const std::string& username)
{
    try {
        UserRequest req;
        req.username = username;

        auto r = cpr::Post(
            cpr::Url{ baseUrl + "/profile" },
            cpr::Body{ json(req).dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );

        if (r.text.empty()) {
            ProfileResponse err;
            err.status = "error";
            err.message = "Server not responding";
            return err;
        }

        if (r.status_code == 200)
            return json::parse(r.text).get<ProfileResponse>();

        try {
            return json::parse(r.text).get<ProfileResponse>();
        }
        catch (...) {
            ProfileResponse err;
            err.status = "error";
            err.message = "Failed to retrieve profile";
            return err;
        }
    }
    catch (const std::exception& e) {
        ProfileResponse err;
        err.status = "error";
        err.message = "Exception in GetProfile: " + std::string(e.what());
        return err;
    }
}

void ClientApi::SetActiveGame(int gameId)
{
    m_activeGameId = gameId;
}

int ClientApi::GetActiveGameId() const
{
    return m_activeGameId;
}

void ClientApi::ResetGame()
{
    m_activeGameId = -1;
}

BasicResponse ClientApi::HandleResponse(const cpr::Response& r, const std::string& actionName)
{
    if (r.text.empty())
        return { "error", "Server not responding during " + actionName };

    if (r.status_code != 200)
    {
        try {
            return json::parse(r.text).get<BasicResponse>();
        }
        catch (...)
        {
            return { "error", actionName + " failed with code " + std::to_string(r.status_code) };
        }
    }

    try {
        return json::parse(r.text).get<BasicResponse>();
    }
    catch (const std::exception& e) {
        return { "error", "Invalid JSON in " + actionName + ": " + std::string(e.what()) };
    }
}





