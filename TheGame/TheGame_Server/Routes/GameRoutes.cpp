#include "GameRoutes.h"
#include "GameModels.h"
#include "ResponseUtils.h"

void registerGameRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame)
{
	CROW_ROUTE(app, "/gameState/<int>")([&activeGame](int playerIndex) {
		if (activeGame == nullptr)
			return utils::Error(400, "Game has not started yet");

		if (playerIndex < 0 || playerIndex >= activeGame->GetPlayers().size())
			return utils::Error(400, "Invalid player ID");

		std::string gameStateJson = activeGame->GetGameStateAsJson(playerIndex);

		return crow::response(gameStateJson);
		});

	CROW_ROUTE(app, "/playCard").methods("POST"_method)
		([&activeGame](const crow::request& request) {
		if (activeGame == nullptr)
			return utils::Error(400, "Game has not started yet");

		try {
			auto action = json::parse(request.body).get<PlayCardAction>();

			bool success = activeGame->PlayCard(action.playerIndex, action.handIndex, action.stackIndex);

			if (success) 
				return utils::Success("Successfully played card");
			else 
				return utils::Error(400, "Rules broken or not your turn");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});


	CROW_ROUTE(app, "/endTurn").methods("POST"_method)
		([&activeGame](const crow::request& request) {
		if (activeGame == nullptr)
			return utils::Error(400, "Game has not started yet");

		try {
			auto action = json::parse(request.body).get<EndTurnAction>();

			bool success = activeGame->EndTurn(action.playerIndex);

			if (success)
				return utils::Success("Successfully ended turn");
			else 
				return utils::Error(400, "Cannot end turn");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});
}