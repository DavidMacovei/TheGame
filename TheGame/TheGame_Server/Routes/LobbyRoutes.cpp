#include "LobbyRoutes.h"
#include "ResponseUtils.h"
#include "GameModels.h"

void registerLobbyRoutes(crow::SimpleApp& app, game::GameManager& gameManager)
{
	CROW_ROUTE(app, "/joinLobby").methods("POST"_method)
		([&gameManager](const crow::request& request) {
		try {
			auto req = json::parse(request.body).get<UserRequest>();

			if (req.username.empty()) {
				return utils::Error(400, "Username required");
			}

			int userScore = 1; //Database logic

			gameManager.AddPlayerToQueue(req.username, userScore);
			gameManager.TryMatchmaking();

			return utils::Success("Joined lobby queue successfully");
		}
		catch (...) {
			return utils::Error(400, "Invaild JSON format");
		}
			});

	CROW_ROUTE(app, "/checkStatus").methods("POST"_method)
		([&gameManager](const crow::request& request) {
		try {
			auto req = json::parse(request.body).get<UserRequest>();

			if (req.username.empty()) {
				return utils::Error(400, "Username required");
			}

			int gameId = gameManager.GetGameIdForPlayer(req.username);
			auto waitingList = gameManager.GetWaitingList();

			UserStatusResponse response;

			if (gameId != -1)
			{
				response.status = "running";
				response.message = "Game is running!";
				response.gameId = gameId;
			}
			else
			{
				bool isInQueue = gameManager.IsPlayerInQueue(req.username);

				if (isInQueue)
				{
					auto waitingListNames = gameManager.GetWaitingList();

					response.status = "waiting";
					response.message = "Waiting for match ...";
					response.playersInQueue = (int)waitingListNames.size();
					
					if (waitingListNames.size() <= 10)
						response.waitingList = waitingListNames;
				}
				else
				{
					response.status = "idle";
					response.message = "Not in lobby";
				}
			}

			return crow::response(200, json(response).dump());
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});
}