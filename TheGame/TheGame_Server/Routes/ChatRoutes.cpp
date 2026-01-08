#include "ChatRoutes.h"
#include "ResponseUtils.h"
#include "GameModels.h"

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat, game::GameManager& gameManager)
{
	CROW_ROUTE(app, "/game/<int>/chat/send").methods("POST"_method)
		([&chat, &gameManager](const crow::request& req, int gameId) {
		try {
			auto game = gameManager.GetGame(gameId);
			if (!game) {
				return utils::Error(404, "Game not found");
			}

			auto msgReq = json::parse(req.body).get<ChatMessageRequest>();

			if (msgReq.sender.empty() || msgReq.message.empty())
				return utils::Error(400, "Missing sender or message");

			if(!game->IsPlayerInGame(msgReq.sender))
				return utils::Error(403, "Access denied. You are not in this game.");

			chat.AddMessage(gameId, msgReq.sender, msgReq.message);

			return utils::Success("Message sent");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});

	CROW_ROUTE(app, "/game/<int>/chat/history").methods("GET"_method)
		([&chat, &gameManager](int gameId) {
		auto game = gameManager.GetGame(gameId);
		if (!game) {
			return utils::Error(404, "Game not found");
		}

		auto messages = chat.GetMessages(gameId);

		ChatHistory history;
		history.messages.assign(messages.begin(), messages.end());

		return crow::response(200, json(history).dump());
			});

}