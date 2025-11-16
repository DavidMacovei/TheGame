#pragma once
#include <string>

class ApiClient
{
public:
    explicit ApiClient(const std::string& baseUrl = "http://127.0.0.1:5000");

    // Posts to /login with the provided username. Returns true on success (HTTP 200), false otherwise.
    bool login(const std::string& user);

private:
    std::string baseUrl;
};

