#include "GameRoutes.h"
#include "../TheGame_Common/GameModels.h"

void registerGameRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame)
{
	CROW_ROUTE(app, "/gameState/<int>")([&activeGame](int playerIndex) {
		if (activeGame == nullptr)
		{
			BasicResponse response;
			response.status = "error";
			response.message = "Game has not started yet";
			return crow::response(400, json(response).dump());
		}

		if (playerIndex < 0 || playerIndex >= activeGame->GetPlayers().size())
		{
			BasicResponse response;
			response.status = "error";
			response.message = "Invalid player ID";
			return crow::response(400, json(response).dump());
		}

		std::string gameStateJson = activeGame->GetGameStateAsJson(playerIndex);

		return crow::response(gameStateJson);
		});

	CROW_ROUTE(app, "/playCard").methods("POST"_method)
		([&activeGame](const crow::request& request) {
		if (activeGame == nullptr)
		{
			BasicResponse response;
			response.status = "error";
			response.message = "Game has not started yet";
			return crow::response(400, json(response).dump());
		}

		try {
			auto action = json::parse(request.body).get<PlayCardAction>();

			bool success = activeGame->PlayCard(action.playerIndex, action.handIndex, action.stackIndex);

			BasicResponse response;
			if (success) 
			{
				response.status = "success";
				response.message = "Successfully played card";
				return crow::response(200, json(response).dump());
			}
			else 
			{
				response.status = "error";
				response.message = "Rules broken or not your turn";
				return crow::response(400, json(response).dump());
			}
		}
		catch (...) {
			BasicResponse response;
			response.status = "error";
			response.message = "Invalid JSON format";
			return crow::response(400, json(response).dump());
		}
			});


	CROW_ROUTE(app, "/endTurn").methods("POST"_method)
		([&activeGame](const crow::request& request) {
		if (activeGame == nullptr)
		{
			BasicResponse response;
			response.status = "error";
			response.message = "Game has not started yet";
			return crow::response(400, json(response).dump());
		}

		try {
			auto action = json::parse(request.body).get<EndTurnAction>();

			bool success = activeGame->EndTurn(action.playerIndex);

			BasicResponse response;
			if (success) 
			{
				response.status = "success";
				response.message = "Successfully ended turn";
				return crow::response(200, json(response).dump());
			}
			else 
			{
				response.status = "error";
				response.message = "Cannot end turn";
				return crow::response(400, json(response).dump());
			}
		}
		catch (...) {
			BasicResponse response;
			response.status = "error";
			response.message = "Invalid JSON format";
			return crow::response(400, json(response).dump());
		}
			});
}