#pragma once
#include <string>
#include <ctime>

struct ChatMessage
{
    std::string sender;
    std::string message;
    std::time_t timestamp;
};
