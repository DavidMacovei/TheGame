#include "LobbyRoutes.h"
#include "ResponseUtils.h"
#include "GameModels.h"

const int PLAYERS_NEEDED = 2;

void registerLobbyRoutes(crow::SimpleApp& app, std::unique_ptr<game::Game>& activeGame, std::vector<std::string>& lobbyPlayers)
{
	CROW_ROUTE(app, "/joinLobby").methods("POST"_method)
		([&activeGame, &lobbyPlayers](const crow::request& request) {
        try {
            auto req = json::parse(request.body).get<JoinLobbyRequest>();

            if (req.username.empty()) {
                return utils::Error(400, "Username required");
            }

            if (activeGame != nullptr)
            {
                if (activeGame->GetStatus() == game::GameStatus::Won ||
                    activeGame->GetStatus() == game::GameStatus::Lost)
                {
                    activeGame = nullptr;
                    lobbyPlayers.clear();
                }
                else 
                    return utils::Error(400, "Game already in progress. Please wait!");
            }

            bool alreadyPlaying = false;
            for (const auto& name : lobbyPlayers) {
                if (name == req.username) {
                    alreadyPlaying = true;
                    break;
                }
            }
            if (!alreadyPlaying)
                lobbyPlayers.push_back(req.username);

            if (lobbyPlayers.size() >= PLAYERS_NEEDED)
            {
                activeGame = std::make_unique<game::Game>(lobbyPlayers);
                lobbyPlayers.clear();

                return utils::Success("Game started");
            }

            BasicResponse response;
            response.status = "waiting";
            response.message = "Joined lobby, waiting for players";
            return crow::response(200, json(response).dump());
        }
        catch (...) {
            return utils::Error(400, "Invaild JSON format");
        }
			});

	CROW_ROUTE(app, "/lobbyState")
		([&activeGame, &lobbyPlayers]() {
        LobbyState lobbyState;

        if (activeGame != nullptr)
        {
            lobbyState.status = "Game running";
            lobbyState.currentPlayers = (int)activeGame->GetPlayers().size();

            for (const auto& player : activeGame->GetPlayers())
                lobbyState.players.push_back(player.GetUsername());
        }
        else
        {
            lobbyState.status = "Waiting";
            lobbyState.currentPlayers = (int)lobbyPlayers.size();
            lobbyState.waitingList = lobbyPlayers;
            lobbyState.neededPlayers = PLAYERS_NEEDED;
        }

        return crow::response(200, json(lobbyState).dump());
			});
}