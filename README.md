# The Game - Multiplayer Cooperative Card Game

## Overview

This project is developed as part of the Modern C++ Managing Networking Projects course.

**TheGame** is a multi-project application designed to implement a multiplayer game server and client architecture. It leverages modern C++ features, object-oriented programming (OOP) principles, and several powerful libraries and frameworks to deliver a robust and scalable system. The solution is composed of three distinct projects:

1. **TheGame_Server**: A RESTful API server built using the Crow framework to manage game sessions, player interactions, and chat functionality.
2. **TheGame_Client**: A Qt-based graphical client application that allows players to interact with the game server.
3. **TheGame_Common**: A shared library that contains common utilities, models, and logic used by both the server and client projects.

---

## 👨‍💻 Team & My Specific Contributions

This project was developed over 10 weeks by a team of 4 students. While the development of both the server and the Qt-based GUI was a collaborative effort, **my primary role was Backend Architecture & Integration Lead**. 

Focusing exclusively on `TheGame_Server` and `TheGame_Common`, my specific contributions included:
* **Architectural Refactoring & OOP Enforcement:** Conducted extensive code revisions and refactored core backend modules to enforce strict Object-Oriented Programming principles, ensuring a scalable and maintainable codebase.
* **System Integration:** Acted as the primary integrator for the server-side logic, assembling individual team contributions into a cohesive, robust RESTful API using the **Crow framework**.
* **Shared Infrastructure:** Developed and maintained the `TheGame_Common` library, standardizing the communication models (using `nlohmann-json`) to ensure seamless data exchange between our backend and the client GUI.

---

## Architecture

### 1. **TheGame_Server**
The server is the backbone of the solution, responsible for handling game logic, player management, and communication. It is built using the **Crow framework**, which provides a lightweight and efficient way to create RESTful APIs in C++. Key features include:

- **RESTful API**: The server exposes endpoints for game management, player actions, and chat functionality. For example:
  - `/game/<int>/chat/send`: Allows players to send messages in a game.
  - `/game/<int>/chat/history`: Retrieves the chat history for a specific game.
- **C++ OOP Architecture**: The server uses classes such as `GameManager` and `Game` to encapsulate game logic and manage player sessions.
- **Middleware Support**: Crow's middleware capabilities are utilized for tasks like authentication and logging.
- **JSON Data Exchange**: The server uses JSON for communication with clients, ensuring a lightweight and human-readable data format.

### 2. **TheGame_Client**
The client is a **Qt-based GUI application** that provides an intuitive interface for players to interact with the game. Key features include:

- **User Interface**: Built using Qt Designer, the client offers a clean and responsive UI for players to join games, send messages, and view game status.
- **HTTP Communication**: The client communicates with the server via HTTP requests, sending and receiving JSON data.
- **Event Handling**: Qt's signal-slot mechanism is used to handle user interactions and update the UI dynamically.

### 3. **TheGame_Common**
The common project is a shared library that provides reusable components and logic for both the server and client projects. Key features include:

- **Shared Models**: Defines data models such as `ChatMessageRequest` and `ChatHistory` that are used for JSON serialization and deserialization.
- **Utility Functions**: Provides helper functions for error handling, logging, and data validation.
- **Centralized Logic**: Ensures consistency between the server and client by centralizing shared logic in one place.

---

## Key Technologies

- **C++ OOP**: The solution is designed with a strong emphasis on object-oriented principles, ensuring modularity, reusability, and maintainability.
- **Crow Framework**: A modern C++ framework for building RESTful APIs, used extensively in the server project.
- **SQLite & sqlite-orm**: Leveraged for persistent database storage using modern Object-Relational Mapping to seamlessly map C++ classes to relational tables.
- **JSON**: Used for data exchange between the server and client, ensuring interoperability and ease of debugging.
- **Qt**: A powerful framework for building cross-platform GUI applications, used in the client project.

---

## 🛠️ How to Build and Run (Visual Studio)

### Prerequisites
* **Visual Studio 2022** (with "Desktop development with C++" and Windows SDK installed)
* **C++17 / C++20 compatible compiler** (MSVC)
* **Qt6 development libraries** (configured via Qt Visual Studio Tools)
* **vcpkg** package manager (required dependencies: `crow`, `cpr`, `nlohmann-json`, `sqlite-orm`)

### Build Steps
1. Clone the repository: `git clone https://github.com/DavidMacovei/TheGame.git`
2. Open the solution file `TheGame.sln` in Visual Studio.
3. Ensure the required libraries are restored via vcpkg or your package manager.
4. Set the build configuration to **x64 Debug** or **Release**.
5. Build the entire solution (`Ctrl + Shift + B`).

### Running the Application
1. Navigate to the build output directory (e.g., `x64/Debug`).
2. **Start the Server:** Launch `TheGame_Server.exe` to initialize the API and database.
3. **Start the Client:** Launch `TheGame_Client.exe` to open the GUI.
4. Register a new player, log in, and join a game session!
---

## License

This project is for educational purposes.

---

## Video Demo
https://github.com/user-attachments/assets/86f919c9-30c1-4b0c-8603-fabf1103bf22

