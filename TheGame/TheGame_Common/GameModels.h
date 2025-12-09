#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <array>
#include <nlohmann/json.hpp>

#ifdef THEGAME_COMMON_EXPORTS
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

using json = nlohmann::json;

const uint8_t numberOfStacks = 4;

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
	std::string username;
	std::vector<int> hand;
	int cardCount = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerState, username, hand, cardCount)
};

struct COMMON_API StackState {
	int topCardValue;
	bool isAscending;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(StackState, topCardValue, isAscending)
};

struct COMMON_API GameState {
	std::string status;
	int currentPlayer = -1;
	std::vector<PlayerState> players;
	std::array<StackState, numberOfStacks> placingStacks;
	int drawDeckCount = 0;
	int minCardsToPlay = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameState, status, currentPlayer, players, placingStacks, drawDeckCount, minCardsToPlay)
};

struct COMMON_API PlayCardAction {
	int playerIndex;
	int handIndex;
	int stackIndex;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayCardAction, playerIndex, handIndex, stackIndex)
};

struct COMMON_API EndTurnAction {
	int playerIndex;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(EndTurnAction, playerIndex)
};

struct COMMON_API ChatMessageRequest {
	std::string sender;
	std::string message;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ChatMessageRequest, sender, message)
};

struct COMMON_API ChatMessage {
	std::string sender;
	std::string message;
	std::time_t timestamp;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ChatMessage, sender, message, timestamp)
};

struct COMMON_API ChatHistory {
	std::vector<ChatMessage> messages;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ChatHistory, messages)
};

void COMMON_API InitGameModels();

