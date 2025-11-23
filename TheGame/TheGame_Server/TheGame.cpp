#include <iostream>
#include <crow.h>

#include "Game.h"

#include <crow.h>
#include "UsersDatabase.h"
#include "Chat.h"



int main()
{
	crow::SimpleApp app;

	std::vector<std::string> playerNames = { "Jucator1", "Jucator2", "Jucator3" };
	Game game(playerNames);

	CROW_ROUTE(app, "/")([](){
		return "Hello!";
		});

	CROW_ROUTE(app, "/gameState/<int>")([&game](int playerIndex) {
		if (playerIndex < 0 || playerIndex >= game.GetPlayers().size())
			return crow::response("400", "Invalid player ID");

		crow::json::wvalue gameStateJson = game.GetGameStateAsJson(playerIndex);

		return crow::response(gameStateJson);
		});

	CROW_ROUTE(app, "/playCard").methods("POST"_method)
		([&game](const crow::request& request){
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

		bool success = game.PlayCard(playerIndex, handIndex, stackIndex);

		if (success)
		{
			crow::json::wvalue response;
			response["status"] = "Successfully played card";
			response["newGameState"] = game.GetGameStateAsJson(playerIndex);
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
		([&game](const crow::request& request) {
		crow::json::rvalue turnData = crow::json::load(request.body);

		if (!turnData.has("playerIndex"))
		{
			crow::json::wvalue response;
			response["status"] = "Error";
			response["error"] = "Incomplete JSON: playerIndex is required.";
			return crow::response(400, response);
		}

		uint8_t playerIndex = turnData["playerIndex"].i();

		bool success = game.EndTurn(playerIndex);

		if (success)
		{
			crow::json::wvalue response;
			response["status"] = "Successfully ended turn";

			int nextPlayerID = game.GetCurrentPlayerIndex();
			response["newGameState"] = game.GetGameStateAsJson(nextPlayerID);

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
		([](const crow::request& req) {
		auto body = crow::json::load(req.body);

		if (!body || !body.has("sender") || !body.has("message"))
		{
			crow::json::wvalue res;
			res["status"] = "Error";
			res["error"] = "Missing sender or message";
			return crow::response(400, res);
		}

		ChatMessage msg;
		msg.sender = body["sender"].s();
		msg.message = body["message"].s();
		msg.timestamp = std::time(nullptr);

		{
			std::lock_guard<std::mutex> lock(g_chatMutex);

			g_chatMessages.push_back(msg);

			if (g_chatMessages.size() > MAX_CHAT_MESSAGES)
			{
				g_chatMessages.erase(
					g_chatMessages.begin(),
					g_chatMessages.begin() + (g_chatMessages.size() - MAX_CHAT_MESSAGES)
				);
			}
		}

		crow::json::wvalue res;
		res["status"] = "ok";
		return crow::response(200, res);
			});

	CROW_ROUTE(app, "/getMessages").methods("GET"_method)
		([]() {
		crow::json::wvalue res;

		std::lock_guard<std::mutex> lock(g_chatMutex);

		const std::size_t N = 20;
		std::size_t count = g_chatMessages.size();
		std::size_t start = (count > N ? count - N : 0);

		crow::json::wvalue::list list;

		for (std::size_t i = start; i < count; ++i)
		{
			crow::json::wvalue msg;
			msg["sender"] = g_chatMessages[i].sender;
			msg["message"] = g_chatMessages[i].message;
			msg["timestamp"] = (long long)g_chatMessages[i].timestamp;

			list.push_back(msg);
		}

		res["messages"] = std::move(list);

		return crow::response(200, res);
		});

    app.port(18080).multithreaded().run();

	return 0;
}