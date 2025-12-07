#pragma once
#include<iostream>
#include<vector>
#include<cpr/cpr.h>
#include<crow/json.h>

struct PlayerState {
    std::string username;
    std::vector<int> hand; // card values
};

struct GameState {
    std::string status;
    int currentPlayer = -1;
    std::vector<PlayerState> players;
    std::vector<std::vector<int>> stacks;
    int drawDeckCount = 0;
    int minCardsToPlay = 0;
};

struct LobbyStatus {
    bool gameStarted = false;
    int playerCount = 0;
    int maxPlayers = 4;
    std::vector<std::string> playerNames;
};


class ClientApi
{
public:
    // Posts to /login with the provided username. Returns true on success (HTTP 200), false otherwise.
    bool login(const std::string& user);

    // POSTs to /register with username and password. Returns true on success (HTTP 200/201), false otherwise.
    bool registerUser(const std::string& username, const std::string& password);

    // POSTs to /joinLobby with username. Returns true on success (HTTP 200/201), false otherwise.
    bool joinLobby(const std::string& username);

    // GETs the current game state from the server and returns a parsed GameState.
    GameState getGameState();

    // GETs the lobby status from the server (has game started, how many players).
    LobbyStatus getLobbyStatus();
private:
    std::string baseUrl;
};

