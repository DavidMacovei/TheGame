#include "ConsoleUI.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

ConsoleUI::ConsoleUI(ApiClient& client) : apiClient(client) {}

void ConsoleUI::Run() {
    while (true) {
        if (!clientState.isLoggedIn) {
            ShowMainMenu();
        }
        else if (clientState.inLobby) {
            RunLobbyLoop();
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
    std::cout << "3.  Exit" << std::endl;
    std::cout << "\nAlege o optiune: ";

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
        std::cout << "La revedere!" << std::endl;
        exit(0);
    default:
        std::cout << "Optiune invalida.  Incearca din nou." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        break;
    }
}

void ConsoleUI::HandleLogin() {
    PrintHeader("Login");
    std::string user;
    std::cout << "Username: ";
    std::cin >> user;

    std::cout << "\nSe conecteaza la server..." << std::endl;

    if (apiClient.login(user)) {
        std::cout << "\n[OK] Login reusit!" << std::endl;

        std::cout << "Se intra in lobby..." << std::endl;
        if (apiClient.joinLobby(user)) {
            std::cout << "[OK] Ai intrat in lobby!" << std::endl;
            clientState.isLoggedIn = true;
            clientState.username = user;
            clientState.inLobby = true;
            clientState.cardsPlayedThisTurn = 0;
        }
        else {
            std::cout << "[EROARE] Nu s-a putut intra in lobby." << std::endl;
        }
    }
    else {
        std::cout << "\n[EROARE] Login esuat.  Serverul nu a putut autentifica utilizatorul." << std::endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    WaitForEnter();
}

void ConsoleUI::HandleRegister() {
    PrintHeader("Register");
    std::string user, pass1, pass2;

    std::cout << "Alege un username: ";
    std::cin >> user;

    std::cout << "Alege o parola: ";
    std::cin >> pass1;

    std::cout << "Confirma parola: ";
    std::cin >> pass2;

    if (pass1 != pass2) {
        std::cout << "\n[EROARE] Parolele nu se potrivesc!" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        WaitForEnter();
        return;
    }

    std::cout << "\nSe inregistreaza..." << std::endl;

    if (apiClient.registerUser(user, pass1)) {
        std::cout << "\n[OK] Inregistrare reusita!  Acum te poti loga." << std::endl;
    }
    else {
        std::cout << "\n[EROARE] Inregistrare esuata. Username-ul poate fi deja folosit." << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    WaitForEnter();
}

void ConsoleUI::RunLobbyLoop() {
    const int POLL_INTERVAL_SEC = 3;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (clientState.isLoggedIn && clientState.inLobby) {
        LobbyStatus status = apiClient.getLobbyStatus();

        DisplayLobbyState(status);

        if (status.gameStarted) {
            std::cout << "\n  >>> JOCUL A INCEPUT! <<<\n" << std::endl;
            clientState.inLobby = false;
            clientState.cardsPlayedThisTurn = 0;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }

        std::cout << "\n[R] Refresh   [Q] Delogare\n";
        std::cout << "(Auto-refresh in " << POLL_INTERVAL_SEC << " secunde)\n";
        std::cout << "Comanda: ";
        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::seconds(POLL_INTERVAL_SEC));

        if (std::cin.rdbuf()->in_avail() > 0) {
            std::string line;
            std::getline(std::cin, line);
            if (!line.empty()) {
                char cmd = std::toupper(line[0]);
                if (cmd == 'Q') {
                    std::cout << "Te deloghezi..." << std::endl;
                    clientState.isLoggedIn = false;
                    clientState.inLobby = false;
                    break;
                }
            }
        }
    }
}

void ConsoleUI::DisplayLobbyState(const LobbyStatus& status) {}

void ConsoleUI::RunGameLoop() {
    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (clientState.isLoggedIn && !clientState.inLobby) {
        GameState gs = apiClient.getGameState();

        if (gs.players.empty() && gs.stacks.empty()) {
            ClearScreen();
            PrintHeader("Eroare Conexiune");
            std::cout << "[! ] Nu s-a putut obtine starea jocului de la server.\n" << std::endl;
            std::cout << "[R] Reincearca   [Q] Delogare\n";
            std::cout << "Comanda: ";

            std::string line;
            std::getline(std::cin, line);
            if (!line.empty() && std::toupper(line[0]) == 'Q') {
                clientState.isLoggedIn = false;
                break;
            }
            continue;
        }

        if (gs.status == "won" || gs.status == "lost" || gs.status == "finished") {
            ClearScreen();
            PrintHeader("JOC TERMINAT");
            if (gs.status == "won") {
                std::cout << "  *** FELICITARI! ATI CASTIGAT!  ***\n" << std::endl;
            }
            else {
                std::cout << "  --- Din pacate, ati pierdut.  ---\n" << std::endl;
            }
            WaitForEnter("Apasa Enter pentru a reveni la meniu.. .");
            clientState.isLoggedIn = false;
            clientState.inLobby = true;
            break;
        }

        DisplayGameState(gs);

        std::string currentPlayerName;
        if (gs.currentPlayer >= 0 && gs.currentPlayer < static_cast<int>(gs.players.size())) {
            currentPlayerName = gs.players[gs.currentPlayer].username;
        }

        PrintSeparator();

        bool isMyTurn = (!currentPlayerName.empty() && currentPlayerName == clientState.username);

        if (isMyTurn) {
            std::cout << ">>> ESTE RANDUL TAU!  <<<" << std::endl;
            std::cout << "Carti jucate in aceasta tura: " << clientState.cardsPlayedThisTurn;
            std::cout << " (minim " << gs.minCardsToPlay << ")\n" << std::endl;
            std::cout << "[J] Joaca o carte   [E] End Turn   [C] Chat   [R] Refresh   [Q] Delogare\n";
        }
        else {
            if (!currentPlayerName.empty()) {
                std::cout << "Randul lui: " << currentPlayerName << "\n" << std::endl;
            }
            std::cout << "[C] Chat   [R] Refresh   [Q] Delogare\n";
        }

        std::cout << "Comanda: ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        char cmd = std::toupper(line[0]);

        switch (cmd) {
        case 'J':
            if (isMyTurn) {
                HandlePlayerTurn(gs);
            }
            else {
                std::cout << "\n[!] Nu este randul tau!" << std::endl;
                WaitForEnter();
            }
            break;

        case 'E':
            if (isMyTurn) {
                HandleEndTurn();
            }
            else {
                std::cout << "\n[!] Nu este randul tau!" << std::endl;
                WaitForEnter();
            }
            break;

        case 'R':
            break;

        case 'Q':
            std::cout << "\nTe deloghezi..." << std::endl;
            clientState.isLoggedIn = false;
            clientState.inLobby = true;
            break;

        default:
            std::cout << "\n[!] Comanda necunoscuta: " << cmd << std::endl;
            WaitForEnter();
            break;
        }
    }
}

void ConsoleUI::DisplayGameState(const GameState& state) {
    ClearScreen();
    PrintHeader("THE GAME - Tabla de Joc");

    std::cout << "Carti in pachet: " << state.drawDeckCount;
    std::cout << "  |  Min carti de jucat: " << state.minCardsToPlay;
    if (!state.status.empty() && state.status != "playing") {
        std::cout << "  |  Status: " << state.status;
    }
    std::cout << "\n" << std::endl;

    DisplayStacks(state);

    std::cout << "\n--- JUCATORI ---\n" << std::endl;

    for (size_t i = 0; i < state.players.size(); ++i) {
        const PlayerState& p = state.players[i];
        bool isMe = (p.username == clientState.username);
        bool isCurrentPlayer = (static_cast<int>(i) == state.currentPlayer);

        if (isCurrentPlayer) {
            std::cout << " >> ";
        }
        else {
            std::cout << "    ";
        }

        if (isMe) {
            std::cout << "[TU] ";
        }
        std::cout << p.username;
        std::cout << " - " << p.hand.size() << " carti";

        if (isCurrentPlayer) {
            std::cout << "  [JOACA ACUM]";
        }
        std::cout << std::endl;

        if (isMe) {
            DisplayPlayerHand(p);
        }
    }
}

void ConsoleUI::DisplayStacks(const GameState& state){}

void ConsoleUI::DisplayPlayerHand(const PlayerState& player) {}

void ConsoleUI::HandlePlayerTurn(const GameState& player) {}

void ConsoleUI::HandleEndTurn() {}

void ConsoleUI::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUI::PrintHeader(const std::string& title) {
    ClearScreen();
    std::cout << "==========================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "==========================================" << std::endl << std::endl;
}

void ConsoleUI::PrintSeparator() {
    std::cout << "\n------------------------------------------\n" << std::endl;
}

void ConsoleUI::WaitForEnter(const std::string& message) {
    std::cout << "\n" << message;
    std::cin.get();
}