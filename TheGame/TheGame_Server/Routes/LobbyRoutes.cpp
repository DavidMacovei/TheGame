#include "LobbyRoutes.h"
#include "ResponseUtils.h"
#include "GameModels.h"
#include "UsersDatabase.h"
#include "Logger.h"

void registerLobbyRoutes(crow::SimpleApp& app, game::GameManager& gameManager)
{
	CROW_ROUTE(app, "/lobby/join").methods("POST"_method)
		([&gameManager](const crow::request& request) {
		try {
			auto req = json::parse(request.body).get<UserRequest>();

			if (req.username.empty()) {
				return utils::Error(400, "Username required");
			}

			if (gameManager.IsPlayerInQueue(req.username) || gameManager.GetGameIdForPlayer(req.username) != -1)
				return utils::Error(409, "User already in queue or game");

			int userScore = 1;

			try {
				auto& storage = DatabaseManager::GetInstance().GetStorage();
				auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == req.username));

				if (!users.empty())
					userScore = users[0].GetScore();
			} 
			catch (const std::exception& e) {
				Logger::Error("[DB ERROR] Could not fetch score {}", e.what());
			}

			gameManager.AddPlayerToQueue(req.username, userScore);
			gameManager.TryMatchmaking();

			return utils::Success("Joined lobby queue successfully");
		}
		catch (const std::exception& e) {
			return utils::Error(400, std::string("Bad Request: ") + e.what());
		}
		catch (...) {
			return utils::Error(500, "Internal Server Error");
		}
			});

	CROW_ROUTE(app, "/lobby/status").methods("POST"_method)
		([&gameManager](const crow::request& request) {
		try {
			auto req = json::parse(request.body).get<UserRequest>();

			if (req.username.empty()) {
				return utils::Error(400, "Username required");
			}

			gameManager.TryMatchmaking();

			int gameId = gameManager.GetGameIdForPlayer(req.username);

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
					response.secondsRemaining = gameManager.GetSecondsRemaining();
					
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
		catch (const std::exception& e) {
			return utils::Error(400, std::string("Bad Request: ") + e.what());
		}
		catch (...) {
			return utils::Error(500, "Internal Server Error");
		}
			});

	CROW_ROUTE(app, "/lobby/leave").methods("POST"_method)
		([&gameManager](const crow::request& request) {
		try {
			auto req = json::parse(request.body).get<UserRequest>();

			if (req.username.empty()) {
				return utils::Error(400, "Username required");
			}

			gameManager.RemovePlayerFromQueue(req.username);

			return utils::Success("Left lobby successfully");
		}
		catch (const std::exception& e) {
			return utils::Error(400, std::string("Bad Request: ") + e.what());
		}
		catch (...) {
			return utils::Error(500, "Internal Server Error");
		}
			});
}