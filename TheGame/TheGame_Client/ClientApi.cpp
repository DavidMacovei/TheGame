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
        return { "error", "Exception in register: " + std::string(e.what()) };
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





