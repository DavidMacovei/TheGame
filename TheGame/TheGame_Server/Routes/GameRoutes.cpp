#include "GameRoutes.h"
#include "GameModels.h"
#include "ResponseUtils.h"
#include "GameSerializer.h"


void CheckAndLogGameEnd(const std::shared_ptr<game::Game>& game, int gameId)
{
	auto status = game->GetStatus();
	if (status == game::GameStatus::Won || status == game::GameStatus::Lost)
	{
		std::cout << "[Game " << gameId << "] Finished. Status: "
			<< (status == game::GameStatus::Won ? "WON" : "LOST") << "\n";
	}
}

void registerGameRoutes(crow::SimpleApp& app, game::GameManager& gameManager)
{
	CROW_ROUTE(app, "/game/<int>/state").methods("POST"_method)
		([&gameManager](const crow::request& request, int gameId) {
		try
		{
			auto game = gameManager.GetGame(gameId);
			if (!game)
				return utils::Error(404, "Game not found");

			auto req = json::parse(request.body).get<UserRequest>();

			if (!game->IsPlayerInGame(req.username))
				return utils::Error(403, "Access denied");

			return crow::response(200, game::SerializeGameState(*game, req.username));
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
		});

	CROW_ROUTE(app, "/game/<int>/playCard").methods("POST"_method)
		([&gameManager](const crow::request& request, int gameId) {
		try {
			auto game = gameManager.GetGame(gameId);
			if (!game)
				return utils::Error(404, "Game not found");

			auto action = json::parse(request.body).get<PlayCardAction>();

			bool success = game->PlayCard(action.playerIndex, action.handIndex, action.stackIndex);

			if (success)
			{
				CheckAndLogGameEnd(game, gameId);

				return utils::Success("Card played successfully");
			}

			return utils::Error(400, "Invalid move (rules violation or not your turn)");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});


	CROW_ROUTE(app, "/game/<int>/endTurn").methods("POST"_method)
		([&gameManager](const crow::request& request, int gameId) {
		try {
			auto game = gameManager.GetGame(gameId);
			if (!game)
				return utils::Error(404, "Game not found");

			auto action = json::parse(request.body).get<EndTurnAction>();

			bool success = game->EndTurn(action.playerIndex);

			if (success)	
			{
				CheckAndLogGameEnd(game, gameId);


				return utils::Success("Turn ended");
			}

			return utils::Error(400, "Cannot end turn yet (minimum cards played?)");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});
}