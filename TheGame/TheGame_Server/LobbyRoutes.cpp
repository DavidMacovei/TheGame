#include "LobbyRoutes.h"

void registerLobbyRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame, std::vector<std::string>& lobbyPlayers)
{
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
		for (const auto& name : lobbyPlayers)
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
}