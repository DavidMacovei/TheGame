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
    void run();

private:
    ApiClient& apiClient;
    ClientState clientState;

    void showMainMenu();
    void handleLogin();
    void handleRegister();

    //Functiile acestea necesita GameState
    //void runGameLoop();
    //void displayGameState(const GameState& state);
    //void handlePlayerTurn(const GameState& state);

    void clearScreen();
    void printHeader(const std::string& title);
};