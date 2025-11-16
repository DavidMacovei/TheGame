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


	app.port(18080).multithreaded().run();

	return 0;
}