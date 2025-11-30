#include "ChatRoutes.h"

void registerChatRoutes(crow::SimpleApp& app, ChatService& chat)
{
	CROW_ROUTE(app, "/sendMessage").methods("POST"_method)
		([&chat](const crow::request& req) {
		auto body = crow::json::load(req.body);

		if (!body || !body.has("sender") || !body.has("message"))
		{
			crow::json::wvalue res;
			res["status"] = "Error";
			res["error"] = "Missing sender or message";
			return crow::response(400, res);
		}

		chat.addMessage(body["sender"].s(), body["message"].s());

		crow::json::wvalue res;
		res["status"] = "ok";
		return crow::response(200, res);
			});

	CROW_ROUTE(app, "/getMessages").methods("GET"_method)
		([&chat]() {
		crow::json::wvalue res;

		auto messages = chat.getMessages();

		crow::json::wvalue::list jsonList;

		for (const auto& m : messages)
		{
			crow::json::wvalue msg;
			msg["sender"] = m.sender;
			msg["message"] = m.message;
			msg["timestamp"] = (long long)m.timestamp;

			jsonList.push_back(msg);
		}

		res["messages"] = std::move(jsonList);
		return crow::response(200, res);
			});

}