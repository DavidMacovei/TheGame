#pragma once
#include "GameModels.h"
#include <cpr/cpr.h>

class ClientApi
{
public:
	explicit ClientApi(const std::string& baseUrl = "http://127.0.0.1:18080");

	BasicResponse Login(const std::string& username, const std::string& password);
	BasicResponse RegisterUser(const std::string& username, const std::string& password);

	BasicResponse JoinLobby(const std::string& username);
	BasicResponse LeaveLobby(const std::string& username);
	UserStatusResponse GetUserStatus(const std::string& username);

	GameState GetGameState(const std::string& username);
	BasicResponse PlayCard(int playerIndex, int handIndex, int stackIndex);
	BasicResponse EndTurn(int playerIndex);

	BasicResponse sendMessage(const std::string& sender, const std::string& message);
	ChatHistory GetChatHistory();

    ProfileResponse GetProfile(const std::string& username);

	void SetActiveGame(int gameId);
	int GetActiveGameId() const;
	void ResetGame();

private:
	BasicResponse HandleResponse(const cpr::Response& r, const std::string& actionName);

private:
	std::string m_baseUrl;
	int m_activeGameId = -1;
};

