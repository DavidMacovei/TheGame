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
            RunGameLoop();
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
            std::cout << "Nu s-a putut obtine starea jocului de la server. Apasa Enter pentru a reincerca sau 'q' + Enter pentru a te deloga." << std::endl;
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
            std::cout << "\nEste randul tau si joci!" << std::endl;
            HandlePlayerTurn(gs);
        }
        else {
            if (!currentPlayerName.empty())
                std::cout << "\nMomentan e randul: " << currentPlayerName << std::endl;
            else
                std::cout << "\nServerul nu a specificat jucatorul curent." << std::endl;

            std::cout << "Apasa Enter pentru a reimprospata starea sau tasteaza 'q' + Enter pentru a te deloga." << std::endl;
            std::string line;
            std::getline(std::cin, line);
            if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
                clientState.isLoggedIn = false;
                break;
            }
        }
    }
}

void ConsoleUI::DisplayGameState(const GameState& state) {
    ClearScreen();
    PrintHeader("The Game - Starea curenta");

    if (!state.status.empty()) {
        std::cout << "Server status: " << state.status << std::endl;
    }
    std::cout << "Carti ramase in pachet: " << state.drawDeckCount << std::endl;
    std::cout << "Min carti de jucat: " << state.minCardsToPlay << std::endl;
    std::cout << std::endl;

    std::cout << "--- Stive (stacks) ---" << std::endl;
    for (size_t i = 0; i < state.stacks.size(); ++i) {
        const auto& s = state.stacks[i];
        std::cout << "Stiva [" << i + 1 << "] (size=" << s.size() << "): ";
        if (s.empty()) {
            std::cout << "(goala)";
        }
        else {
            for (size_t j = 0; j < s.size(); ++j) {
                if (j) std::cout << ", ";
                std::cout << s[j];
            }
            std::cout << "  | top=" << s.back();
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "--- Jucatori ---" << std::endl;
    for (size_t i = 0; i < state.players.size(); ++i) {
        const PlayerState& p = state.players[i];
        bool isMe = (p.username == clientState.username);
        std::cout << (isMe ? "-> " : "   ");
        std::cout << p.username << " | carti in mana: " << p.hand.size();
        if (isMe) {
            std::cout << "  (detalii mana: ";
            for (size_t k = 0; k < p.hand.size(); ++k) {
                if (k) std::cout << ", ";
                std::cout << "[" << k << "]=" << p.hand[k];
            }
            std::cout << ")";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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