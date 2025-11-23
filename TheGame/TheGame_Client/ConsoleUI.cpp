#include "ConsoleUI.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

ConsoleUI::ConsoleUI(ApiClient& client) : apiClient(client) {}

void ConsoleUI::Run() {
    while (true) {
        if (!clientState.isLoggedIn) {
            ShowMainMenu();
        }
        else {
            //runGameLoop(); Functie neimplementata pentru moment
        }
    }
}

void ConsoleUI::ShowMainMenu() {
    PrintHeader("Bun Venit la The Game!");
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Alege o optiune: ";

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        choice = 0;
    }

    switch (choice) {
    case 1:
        HandleLogin();
        break;
    case 2:
        HandleRegister();
        break;
    case 3:
        exit(0);
    default:
        std::cout << "Optiune invalida. Incearca din nou." << std::endl;
        std::cin.get();
        break;
    }
}

void ConsoleUI::HandleLogin() {
    PrintHeader("Login");
    std::string user;
    std::cout << "Username: ";
    std::cin >> user;

    if (apiClient.login(user)) {
        std::cout << "Login reusit! Intri in joc..." << std::endl;
        clientState.isLoggedIn = true;
        clientState.username = user;
    }
    else {
        std::cout << "Login esuat. Serverul nu a putut autentifica utilizatorul." << std::endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void ConsoleUI::HandleRegister() {
    PrintHeader("Register");
    std::cout << "Functionalitatea de inregistrare nu este inca disponibila." << std::endl;
    std::cout << "Asteptam implementarea de catre echipa de Backend." << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void ConsoleUI::RunGameLoop() {

    while (clientState.isLoggedIn) {
        GameState gs = apiClient.getGameState();

        if (gs.players.empty() && gs.stacks.empty()) {
            std::cout << "Nu s-a putut ob?ine starea jocului de la server. Apas? Enter pentru a reîncerca sau 'q' + Enter pentru a te deloga." << std::endl;
            std::string line;
            std::getline(std::cin, line);
            if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
                clientState.isLoggedIn = false;
                break;
            }
            continue;
        }

        DisplayGameState(gs);

        std::string currentPlayerName;
        if (gs.currentPlayer >= 0 && gs.currentPlayer < static_cast<int>(gs.players.size())) {
            currentPlayerName = gs.players[gs.currentPlayer].username;
        }
        else if (!gs.players.empty()) {
            currentPlayerName = gs.players[0].username;
        }
        else {
            currentPlayerName = "";
        }

        if (!currentPlayerName.empty() && currentPlayerName == clientState.username) {
            std::cout << "\nEste rândul t?u s? joci!" << std::endl;
            HandlePlayerTurn(gs);
        }
        else {
            if (!currentPlayerName.empty())
                std::cout << "\nMomentan e rândul: " << currentPlayerName << std::endl;
            else
                std::cout << "\nServerul nu a specificat juc?torul curent." << std::endl;

            std::cout << "Apas? Enter pentru a reîmprosp?ta starea sau tasteaz? 'q' + Enter pentru a te deloga." << std::endl;
            std::string line;
            std::getline(std::cin, line);
            if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
                clientState.isLoggedIn = false;
                break;
            }
        }
    }
}

void ConsoleUI::DisplayGameState(const GameState& state)
{
}

void ConsoleUI::HandlePlayerTurn(const GameState& state)
{
}

void ConsoleUI::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUI::PrintHeader(const std::string& title) {
    ClearScreen();
    std::cout << "=========================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "=========================================" << std::endl << std::endl;
}