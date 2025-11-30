#include "ChatService.h"

void ChatService::addMessage(const std::string& sender, const std::string& message)
{
    ChatMessage msg{ sender, message, std::time(nullptr) };

    std::lock_guard<std::mutex> lock(mutex);

    messages.push_back(msg);

    if (messages.size() + 1 > MAX_CHAT_MESSAGES)
        messages.pop_front();
}

std::deque<ChatMessage> ChatService::getMessages()
{
    std::lock_guard<std::mutex> lock(mutex);
    return messages;
}