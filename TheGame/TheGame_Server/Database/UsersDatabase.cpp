#include "UsersDatabase.h"

DatabaseManager& DatabaseManager::GetInstance()
{
    static DatabaseManager instance;
    return instance;
}

StorageType& DatabaseManager::GetStorage()
{
    return m_storage;
}

DatabaseManager::DatabaseManager() : m_storage(CreateStorage("users.sqlite"))
{
    m_storage.sync_schema();
}
