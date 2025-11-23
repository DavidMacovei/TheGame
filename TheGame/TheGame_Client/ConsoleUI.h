#pragma once
#include "ApiClient.h"
#include <string>

struct ClientState {
    bool isLoggedIn = false;
    std::string username;
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

    void RunGameLoop();
    void DisplayGameState(const GameState& state);
    void HandlePlayerTurn(const GameState& state);

    void ClearScreen();
    void PrintHeader(const std::string& title);
};