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


	app.port(18080).multithreaded().run();

	return 0;
}