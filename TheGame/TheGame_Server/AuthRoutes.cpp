#include "AuthRoutes.h"
#include "UsersDatabase.h"
#include "User.h"
#include "../TheGame_Common/GameModels.h"
#include <functional>

void registerAuthRoutes(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/register").methods("POST"_method)
		([](const crow::request& req) {
		try {
			auto authReq = json::parse(req.body).get<AuthRequest>();

			if (authReq.username.empty() || authReq.password.empty()) {
				BasicResponse response;
				response.status = "error";
				response.message = "Username and password cannot be empty";
				return crow::response(400, json(response).dump());
			}

			std::hash<std::string> hasher;
			std::string password_hash = std::to_string(hasher(authReq.password));

			auto storage = http::CreateStorage("users.sqlite");

			auto existing = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == authReq.username));
			if (!existing.empty()) {
				BasicResponse response;
				response.status = "error";
				response.message = "Username already exists";
				return crow::response(409, json(response).dump());
			}

			User newUser(authReq.username, password_hash);
			storage.insert(newUser);

			BasicResponse response;
			response.status = "success";
			response.message = "Registration successful";
			return crow::response(200, json(response).dump());
		}
		catch (...) {
			BasicResponse response;
			response.status = "error";
			response.message = "Invalid JSON format";
			return crow::response(400, json(response).dump());
		}
			});


	CROW_ROUTE(app, "/login").methods("POST"_method)
		([](const crow::request& req) {
		try {
			auto authReq = json::parse(req.body).get<AuthRequest>();

			if (authReq.username.empty() || authReq.password.empty()) {
				BasicResponse response;
				response.status = "error";
				response.message = "Username and password cannot be empty";
				return crow::response(400, json(response).dump());
			}

			std::hash<std::string> hasher;
			std::string password_hash = std::to_string(hasher(authReq.password));

			auto storage = http::CreateStorage("users.sqlite");

			auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == authReq.username));
			if (users.empty()) {
				BasicResponse response;
				response.status = "error";
				response.message = "Invalid username or password";
				return crow::response(401, json(response).dump());
			}
			if (users[0].GetPasswordHash() != password_hash)
			{
				BasicResponse response;
				response.status = "error";
				response.message = "Invalid username or password";
				return crow::response(401, json(response).dump());
			}

			BasicResponse response;
			response.status = "success";
			response.message = "Login successful";
			return crow::response(200, json(response).dump());
		}
		catch (...) {
			BasicResponse response;
			response.status = "error";
			response.message = "Invalid JSON format";
			return crow::response(400, json(response).dump());
		}
			});
}