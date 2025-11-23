#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <ctime>

struct ChatMessage
{
    std::string sender;
    std::string message;
    std::time_t timestamp;
};

extern std::vector<ChatMessage> g_chatMessages;
extern std::mutex g_chatMutex;

constexpr std::size_t MAX_CHAT_MESSAGES = 100;