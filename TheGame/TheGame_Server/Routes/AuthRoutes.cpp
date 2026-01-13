#include "AuthRoutes.h"
#include "UsersDatabase.h"
#include "User.h"
#include "GameModels.h"
#include "ResponseUtils.h"
#include "GameManager.h"
#include <functional>

void registerAuthRoutes(crow::SimpleApp& app, game::GameManager& gameManager)
{
	CROW_ROUTE(app, "/auth/register").methods("POST"_method)
		([](const crow::request& req) {
		try {
			auto authReq = json::parse(req.body).get<AuthRequest>();

			if (authReq.username.empty() || authReq.password.empty())
				return utils::Error(400, "Username and password cannot be empty");

			std::hash<std::string> hasher;
			std::string password_hash = std::to_string(hasher(authReq.password));

			auto storage = http::CreateStorage("users.sqlite");

			auto existing = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == authReq.username));
			if (!existing.empty())
				return utils::Error(409, "Username already exists");

			User newUser(authReq.username, password_hash);
			storage.insert(newUser);

			return utils::Success("Registration successful");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});


	CROW_ROUTE(app, "/auth/login").methods("POST"_method)
		([&gameManager](const crow::request& req) {
		try {
			auto authReq = json::parse(req.body).get<AuthRequest>();

			if (authReq.username.empty() || authReq.password.empty())
				return utils::Error(400, "Username and password cannot be empty");

			std::hash<std::string> hasher;
			std::string password_hash = std::to_string(hasher(authReq.password));

			auto storage = http::CreateStorage("users.sqlite");

			auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == authReq.username));
			if (users.empty()) 
				return utils::Error(401, "Invalid username or password");

			if (users[0].GetPasswordHash() != password_hash)
				return utils::Error(401, "Invalid username or password");

			if (gameManager.IsPlayerInQueue(authReq.username) || 
			    gameManager.GetGameIdForPlayer(authReq.username) != -1)
				return utils::Error(409, "This account is already logged into on another device.");

			return utils::Success("Login successful");
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});

	CROW_ROUTE(app, "/profile").methods("POST"_method)
		([](const crow::request& req) {
		try {
			auto userReq = json::parse(req.body).get<UserRequest>();

			if (userReq.username.empty())
				return utils::Error(400, "Username required");

			auto storage = http::CreateStorage("users.sqlite");

			auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == userReq.username));
			if (users.empty())
				return utils::Error(404, "User not found");

			ProfileResponse response;
			response.status = "success";
			response.message = "Profile retrieved";
			response.username = users[0].GetUsername();
			response.score = users[0].GetScore();
			response.hoursPlayed = users[0].GetHoursPlayed();

			return crow::response(200, json(response).dump());
		}
		catch (...) {
			return utils::Error(400, "Invalid JSON format");
		}
			});
}