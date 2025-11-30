#pragma once

#include <deque>
#include <mutex>
#include "ChatMessage.h"

class ChatService
{
public:
    ChatService() = default;

    void addMessage(const std::string& sender, const std::string& message);
    std::deque<ChatMessage> getMessages();

private:
    std::deque<ChatMessage> messages;
    std::mutex mutex;

    static constexpr std::size_t MAX_CHAT_MESSAGES = 20;
};