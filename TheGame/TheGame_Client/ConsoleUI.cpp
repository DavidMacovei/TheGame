#include "ConsoleUI.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

ConsoleUI::ConsoleUI(ApiClient& client) : apiClient(client) {}

void ConsoleUI::run() {
    while (true) {
        if (!clientState.isLoggedIn) {
            showMainMenu();
        }
        else {
            //runGameLoop(); Functie neimplementata pentru moment
        }
    }
}

void ConsoleUI::showMainMenu() {
    printHeader("Bun Venit la The Game!");
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
        handleLogin();
        break;
    case 2:
        handleRegister();
        break;
    case 3:
        exit(0);
    default:
        std::cout << "Optiune invalida. Incearca din nou." << std::endl;
        std::cin.get();
        break;
    }
}
