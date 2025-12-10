#include "UsersDatabase.h"

bool http::UserStorage::Initialize()
{
    const std::string dbFile = "users.sqlite";
    bool dbExists = std::filesystem::exists(dbFile);
    auto storage = CreateStorage(dbFile);
    if (!dbExists) {
        storage.sync_schema();
    }
    return true;
}
