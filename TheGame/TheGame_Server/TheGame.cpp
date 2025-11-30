#include <iostream>
#include <crow.h>
#include <memory>
#include "Game.h"
#include "UsersDatabase.h"
#include "ChatService.h"

int main()
{
	crow::SimpleApp app;
	ChatService chat;

	CROW_ROUTE(app, "/ping")
		([]() {
		return "pong";
			});

	CROW_ROUTE(app, "/")
		([]() {
		return "Test route";
			});

	CROW_ROUTE(app, "/test/cpuBenchmark/<int>")
		([](int limit) {
		if (limit > 100000) return crow::response(400, "Limit too high, use < 100000");

		std::vector<int> primes;
		auto start = std::chrono::high_resolution_clock::now();

		std::vector<bool> is_prime(limit + 1, true);
		is_prime[0] = is_prime[1] = false;

		for (int p = 2; p * p <= limit; p++) {
			if (is_prime[p]) {
				for (int i = p * p; i <= limit; i += p)
					is_prime[i] = false;
			}
		}

		for (int p = 2; p <= limit; p++) {
			if (is_prime[p]) {
				primes.push_back(p);
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration = end - start;

		crow::json::wvalue result;
		result["test_name"] = "Prime Number Benchmark";
		result["limit"] = limit;
		result["primes_found"] = (int)primes.size();
		result["execution_time_ms"] = duration.count();

		for (int i = 0; i < 5 && !primes.empty(); i++) {
			result["sample_primes"][i] = primes[primes.size() - 1 - i];
		}

		return crow::response(200, result);
			});

	//TODO: mutex

	std::unique_ptr<game::Game> activeGame = nullptr;
	std::vector<std::string> lobbyPlayers;

	CROW_ROUTE(app, "/gameState/<int>")([&activeGame](int playerIndex) {
		if (activeGame == nullptr)
		{
			return crow::response(400, "Game has not started yet");
		}

		if (playerIndex < 0 || playerIndex >= activeGame->GetPlayers().size())
			return crow::response(400, "Invalid player ID");

		crow::json::wvalue gameStateJson = activeGame->GetGameStateAsJson(playerIndex);

		return crow::response(gameStateJson);
		});

	CROW_ROUTE(app, "/playCard").methods("POST"_method)
		([&activeGame](const crow::request& request){
		if (activeGame == nullptr)
		{
			return crow::response(400, "Game has not started yet");
		}

		crow::json::rvalue cardData = crow::json::load(request.body);

		if (!cardData.has("playerIndex") || !cardData.has("handIndex") || !cardData.has("stackIndex"))
		{
			crow::json::wvalue response;
			response["status"] = "Error";
			response["error"] = "Incomplete JSON: playerIndex, handIndex and stackIndex are required.";
			return crow::response(400, response);
		}

		uint8_t playerIndex = cardData["playerIndex"].i();
		uint8_t handIndex = cardData["handIndex"].i();
		uint8_t stackIndex = cardData["stackIndex"].i();

		bool success = activeGame->PlayCard(playerIndex, handIndex, stackIndex);

		if (success)
		{
			crow::json::wvalue response;
			response["status"] = "Successfully played card";
			response["newGameState"] = activeGame->GetGameStateAsJson(playerIndex);
			return crow::response(200, response);
		}
		else
		{
			crow::json::wvalue response;
			response["status"] = "Invalid move";
			response["error"] = "You cannot play this card (it's not your turn OR you broke the rule)";
			return crow::response(400, response);
		}
		});


	CROW_ROUTE(app, "/endTurn").methods("POST"_method)
		([&activeGame](const crow::request& request) {
		if (activeGame == nullptr)
		{
			return crow::response(400, "Game has not started yet");
		}

		crow::json::rvalue turnData = crow::json::load(request.body);

		if (!turnData.has("playerIndex"))
		{
			crow::json::wvalue response;
			response["status"] = "Error";
			response["error"] = "Incomplete JSON: playerIndex is required.";
			return crow::response(400, response);
		}

		uint8_t playerIndex = turnData["playerIndex"].i();

		bool success = activeGame->EndTurn(playerIndex);

		if (success)
		{
			crow::json::wvalue response;
			response["status"] = "Successfully ended turn";

			return crow::response(200, response);
		}
		else
		{
			crow::json::wvalue response;
			response["status"] = "Ending turn error";
			response["error"] = "It's not your turn OR you haven't played enough cards.";
			return crow::response(400, response);
		}
		});

	CROW_ROUTE(app, "/joinLobby").methods("POST"_method)
		([&activeGame, &lobbyPlayers](const crow::request& request) {
		crow::json::rvalue body = crow::json::load(request.body);

		if (!body.has("username"))
		{
			return crow::response(400, "Username required");
		}
		std::string username = body["username"].s();

		if (activeGame != nullptr)
		{
			if (activeGame->GetStatus() == game::GameStatus::Won ||
				activeGame->GetStatus() == game::GameStatus::Lost)
			{
				activeGame = nullptr;
				lobbyPlayers.clear();
			}
			else
				return crow::response(400, "Game already in progress. Please wait!");
		}

		bool alreadyPlaying = false;
		for(const auto& name : lobbyPlayers)
			if (name == username)
			{
				alreadyPlaying = true;
				break;
			}

		if (!alreadyPlaying)
			lobbyPlayers.push_back(username);

		const int PLAYERS_NEEDED = 4;
		if (lobbyPlayers.size() >= PLAYERS_NEEDED)
		{
			activeGame = std::make_unique<game::Game>(lobbyPlayers);
			lobbyPlayers.clear();

			crow::json::wvalue response;
			response["status"] = "Game started";
			response["players"] = activeGame->GetPlayers().size();
			return crow::response(200, response);
		}

		crow::json::wvalue response;
		response["state"] = "Waiting for players";
		response["current_players"] = lobbyPlayers.size();
		response["needed_players"] = PLAYERS_NEEDED;
		return crow::response(200, response);
		});

	CROW_ROUTE(app, "/lobbyStatus")
		([&activeGame, &lobbyPlayers]() {
		crow::json::wvalue response;

		if (activeGame != nullptr)
		{
			response["status"] = "Game running";
			auto players = activeGame->GetPlayers();
			for (int i = 0; i < players.size(); i++)
				response["players"][i] = players[i].GetUsername();
		}
		else
		{
			response["status"] = "Waiting";
			response["current_players"] = lobbyPlayers.size();
			for (int i = 0; i < lobbyPlayers.size(); i++)
				response["waiting_list"][i] = lobbyPlayers[i];
		}

		return crow::response(200, response);
		});

    http::UserStorage userStorage;
    userStorage.Initialize();

    CROW_ROUTE(app, "/register").methods("POST"_method)
        ([&userStorage](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

		// De schimbat functia de hashing cu una mai sigura
        std::hash<std::string> hasher;


        std::string password_hash = std::to_string(hasher(password));

        auto storage = http::CreateStorage("users.sqlite");

        auto existing = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == username));
        if (!existing.empty()) {
            return crow::response(409, "Username already exists");
        }

        User newUser(username, password_hash);
        storage.insert(newUser);
        return crow::response(200, "Registration successful");
            });


    CROW_ROUTE(app, "/login").methods("POST"_method)
        ([&userStorage](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        std::hash<std::string> hasher;
        std::string password_hash = std::to_string(hasher(password));

        auto storage = http::CreateStorage("users.sqlite");
        auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == username));
        if (users.empty()) {
            return crow::response(401, "Invalid username or password");
        }
        if (users[0].GetPasswordHash() != password_hash) {
            return crow::response(401, "Invalid username or password");
        }

        return crow::response(200, "Login successful");
            });

	CROW_ROUTE(app, "/sendMessage").methods("POST"_method)
		([&chat](const crow::request& req) {
		auto body = crow::json::load(req.body);

		if (!body || !body.has("sender") || !body.has("message"))
		{
			crow::json::wvalue res;
			res["status"] = "Error";
			res["error"] = "Missing sender or message";
			return crow::response(400, res);
		}

		chat.addMessage(body["sender"].s(), body["message"].s());

		crow::json::wvalue res;
		res["status"] = "ok";
		return crow::response(200, res);
			});

	CROW_ROUTE(app, "/getMessages").methods("GET"_method)
		([&chat]() {
		crow::json::wvalue res;

		auto messages = chat.getMessages();

		crow::json::wvalue::list jsonList;

		for (const auto& m : messages)
		{
			crow::json::wvalue msg;
			msg["sender"] = m.sender;
			msg["message"] = m.message;
			msg["timestamp"] = (long long)m.timestamp;

			jsonList.push_back(msg);
		}

		res["messages"] = std::move(jsonList);
		return crow::response(200, res);
			});

    app.port(18080).multithreaded().run();

	return 0;
}