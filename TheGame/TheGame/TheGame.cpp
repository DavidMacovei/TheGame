#include <iostream>

#include <crow.h>
#include "UsersDatabase.h"

int main()
{
    crow::SimpleApp app;
    http::UserStorage userStorage;
    userStorage.Initialize();

    CROW_ROUTE(app, "/register").methods("POST"_method)
        ([&userStorage](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

		// De schimbat functia de hashing cu una mai sigura
        std::hash<std::string> hasher;


        std::string password_hash = std::to_string(hasher(password));

        auto storage = http::CreateStorage("users.sqlite");

        auto existing = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == username));
        if (!existing.empty()) {
            return crow::response(409, "Username already exists");
        }

        User newUser(username, password_hash);
        storage.insert(newUser);
        return crow::response(200, "Registration successful");
            });


    CROW_ROUTE(app, "/login").methods("POST"_method)
        ([&userStorage](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        std::hash<std::string> hasher;
        std::string password_hash = std::to_string(hasher(password));

        auto storage = http::CreateStorage("users.sqlite");
        auto users = storage.get_all<User>(sql::where(sql::c(&User::GetUsername) == username));
        if (users.empty()) {
            return crow::response(401, "Invalid username or password");
        }
        if (users[0].GetPasswordHash() != password_hash) {
            return crow::response(401, "Invalid username or password");
        }

        return crow::response(200, "Login successful");
            });

    app.port(18080).multithreaded().run();

	return 0;
}