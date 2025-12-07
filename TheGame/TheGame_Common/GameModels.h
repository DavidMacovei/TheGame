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

};

struct COMMON_API AuthRequest {

};

struct COMMON_API JoinLobbyRequest {

};

struct COMMON_API LobbyState {

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

