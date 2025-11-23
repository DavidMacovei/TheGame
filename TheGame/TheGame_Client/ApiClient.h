#pragma once
#include <string>
#include <vector>

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

class ApiClient
{
public:
    explicit ApiClient(const std::string& baseUrl = "http://127.0.0.1:5000");

    // Posts to /login with the provided username. Returns true on success (HTTP 200), false otherwise.
    bool login(const std::string& user);

    // GETs the current game state from the server and returns a parsed GameState.
    GameState getGameState();

private:
    std::string baseUrl;
};

