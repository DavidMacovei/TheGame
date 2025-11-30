#include "GameRoutes.h"

void registerGameRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame)
{
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
		([&activeGame](const crow::request& request) {
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
}