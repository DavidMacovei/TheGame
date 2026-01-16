module;

#include <deque>
#include <mutex>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include "GameModels.h"

export module chat;


export class ChatService
{
public:
    ChatService() = default;

    void AddMessage(int gameId, const std::string& sender, const std::string& message);
    std::deque<ChatMessage> GetMessages(int gameId);

    void RemoveChat(int gameId);

private:
    std::map<int, std::deque<ChatMessage>> m_gameChats;
    mutable std::mutex m_mutex;

    static const std::size_t MAX_CHAT_MESSAGES = 20;
};