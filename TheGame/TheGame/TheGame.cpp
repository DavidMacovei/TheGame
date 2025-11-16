#include <iostream>
#include <crow.h>

#include "Game.h"

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

	app.port(18080).multithreaded().run();

	return 0;
}