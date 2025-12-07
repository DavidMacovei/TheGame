#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#ifdef THEGAME_COMMON_EXPORTS
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

using json = nlohmann::json;


struct COMMON_API BasicResponse {
	std::string status;
	std::string error;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BasicResponse, status, error)
};

struct COMMON_API AuthRequest {
	std::string username;
	std::string password;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AuthRequest, username, password)
};

struct COMMON_API JoinLobbyRequest {
	std::string username;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(JoinLobbyRequest, username)
};

struct COMMON_API LobbyState {
	std::string status;
	std::vector<std::string> players;
	std::vector<std::string> waitingList;
	int currentPlayers = 0;
	int neededPlayers = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LobbyState, status, players, waitingList, currentPlayers, neededPlayers)
};

struct COMMON_API PlayerState {

};

struct COMMON_API GameState {

};

struct COMMON_API PlayCardAction {

};

struct COMMON_API EndTurnAction {

};

struct COMMON_API ChatMessageRequest {

};

struct COMMON_API ChatMessage {

};

struct COMMON_API ChatHistory {

};

void COMMON_API InitGameModels();

