#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include "User.h"

namespace sql = sqlite_orm;

inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(filename,
		sql::make_table("Users",
			sql::make_column("id", &User::SetId, &User::GetId, sql::primary_key().autoincrement()),
			sql::make_column("username", &User::SetUsername, &User::GetUsername, sql::unique()),
			sql::make_column("password_hash", &User::SetPasswordHash, &User::GetPasswordHash),
			sql::make_column("score", &User::SetScore, &User::GetScore),
			sql::make_column("hours_played", &User::SetHoursPlayed, &User::GetHoursPlayed),
			sql::make_column("games_played", &User::SetGamesPlayed, &User::GetGamesPlayed),
			sql::make_column("games_won", &User::SetGamesWon, &User::GetGamesWon),
			sql::make_column("total_cards_left_on_loss", &User::SetTotalCardsLeftOnLoss, &User::GetTotalCardsLeftOnLoss)
		)
	);
}

using StorageType = decltype(CreateStorage(""));

class DatabaseManager {
public:
	static DatabaseManager& GetInstance();

	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager& operator=(const DatabaseManager&) = delete;

	StorageType& GetStorage();

private:
	DatabaseManager();

	StorageType m_storage;
};