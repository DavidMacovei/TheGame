#pragma once
#include "ApiClient.h"
#include <string>

struct ClientState {
    bool isLoggedIn = false;
    std::string username;
    bool inLobby = true;
    int cardsPlayedThisTurn = 0;
};

class ConsoleUI {
public:
    ConsoleUI(ApiClient& client);
    void Run();

private:
    ApiClient& apiClient;
    ClientState clientState;

    void ShowMainMenu();
    void HandleLogin();
    void HandleRegister();

    void RunLobbyLoop();
    void DisplayLobbyState(const LobbyStatus& status);

    void RunGameLoop();
    void DisplayGameState(const GameState& state);
    void DisplayStacks(const GameState& state);
    void DisplayPlayerHand(const PlayerState& player);
    void HandlePlayerTurn(const GameState& state);
    void HandleEndTurn();

    void ClearScreen();
    void PrintSeparator();
    void PrintHeader(const std::string& title);
    void WaitForEnter(const std::string& message = "Apasa Enter pentru a continua...");
};