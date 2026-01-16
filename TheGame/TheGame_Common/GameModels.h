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

constexpr int NUMBER_OF_STACKS = 4;	
constexpr int MIN_PLAYERS = 2;
constexpr int MAX_PLAYERS = 5;
constexpr int LOBBY_TIMER_INIT = 30;

struct COMMON_API BasicResponse {
	std::string status;
	std::string message;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BasicResponse, status, message)
};

struct COMMON_API AuthRequest {
	std::string username;
	std::string password;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AuthRequest, username, password)
};

struct COMMON_API UserRequest {
	std::string username;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserRequest, username)
};

struct COMMON_API UserStatusResponse {
	std::string status;
	std::string message;
	int playersInQueue = 0;
	int gameId = -1;
	int secondsRemaining = LOBBY_TIMER_INIT;
	std::vector<std::string> waitingList;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserStatusResponse, status, message, playersInQueue, gameId, secondsRemaining, waitingList)
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
	std::array<StackState, NUMBER_OF_STACKS> placingStacks;
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

struct COMMON_API ProfileResponse {
	std::string status;
	std::string message;
	std::string username;
	int score = 0;
	double hoursPlayed = 0.0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProfileResponse, status, message, username, score, hoursPlayed)
};

