#include "ChatRoutes.h"
#include "ResponseUtils.h"
#include "../TheGame_Common/GameModels.h"

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat)
{
	CROW_ROUTE(app, "/sendMessage").methods("POST"_method)
		([&chat](const crow::request& req) {
		try {
			auto msgReq = json::parse(req.body).get<ChatMessageRequest>();

			if (msgReq.sender.empty() || msgReq.message.empty())
				return utils::Error(400, "Missing sender or message");

			chat.addMessage(msgReq.sender, msgReq.message);

			return utils::Success("Message sent");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});

	CROW_ROUTE(app, "/getMessages").methods("GET"_method)
		([&chat]() {
		ChatHistory history;

		auto messages = chat.getMessages();
		history.messages.assign(messages.begin(), messages.end());

		return crow::response(200, json(history).dump());
			});

}