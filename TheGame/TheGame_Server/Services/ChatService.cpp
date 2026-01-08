#include "ChatService.h"

void ChatService::AddMessage(int gameId, const std::string& sender, const std::string& message)
{
    ChatMessage msg{ sender, message, std::time(nullptr) };

    std::lock_guard<std::mutex> lock(m_mutex);

    auto& currentChat = m_gameChats[gameId];

    currentChat.push_back(msg);

    if (currentChat.size() > MAX_CHAT_MESSAGES)
        currentChat.pop_front();
}

std::deque<ChatMessage> ChatService::GetMessages(int gameId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_gameChats.find(gameId);
    if (it != m_gameChats.end())
        return it->second;
    return {};
}

void ChatService::RemoveChat(int gameId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_gameChats.erase(gameId);
}
