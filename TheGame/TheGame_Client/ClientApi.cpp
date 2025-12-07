#include "ClientApi.h"


// Parse bool field from JSON string
bool parseBoolFieldJson(const std::string& jsonStr, const std::string& fieldName, bool& outValue) {
    try {
        auto obj = crow::json::load(jsonStr);
        if (!obj) return false;
        if (!obj.has(fieldName)) return false;
        outValue = obj[fieldName].b();
        return true;
    } catch (...) {
        return false;
    }
}

// Parse string field from JSON string
bool parseStringFieldJson(const std::string& jsonStr, const std::string& fieldName, std::string& outValue) {
    try {
        auto obj = crow::json::load(jsonStr);
        if (!obj) return false;
        if (!obj.has(fieldName)) return false;
        outValue = obj[fieldName].s();
        return true;
    } catch (...) {
        return false;
    }
}

// Parse int field from JSON string
bool parseIntFieldJson(const std::string& jsonStr, const std::string& fieldName, int& outValue) {
    try {
        auto obj = crow::json::load(jsonStr);
        if (!obj) return false;
        if (!obj.has(fieldName)) return false;
        outValue = obj[fieldName].i();
        return true;
    } catch (...) {
        return false;
    }
}

// Parse int array field from JSON string
bool parseIntArrayFieldJson(const std::string& jsonStr, const std::string& fieldName, std::vector<int>& outArray) {
    try {
        auto obj = crow::json::load(jsonStr);
        if (!obj) return false;
        if (!obj.has(fieldName)) return false;
        auto arr = obj[fieldName];
        if (!arr) return false;
        for (size_t i = 0; i < arr.size(); ++i) {
            outArray.push_back(arr[i].i());
        }
        return true;
    } catch (...) {
        return false;
    }
}

// Posts to /login with the provided username. Returns true on success (HTTP 200), false otherwise.
bool login(const std::string& user) {
    const std::string baseUrl = "http://localhost:18080"; // Change if needed
    try {
        std::string url = baseUrl + "/login";
        std::string body = "{\"username\":\"" + user + "\"}";
        cpr::Response r = cpr::Post(cpr::Url{url},
                                    cpr::Body{body},
                                    cpr::Header{{"Content-Type", "application/json"}});
        return r.status_code == 200;
    } catch (...) {
        return false;
    }
}

// POSTs to /register with username and password. Returns true on success (HTTP 200/201), false otherwise.
bool registerUser(const std::string& username, const std::string& password) {
    const std::string baseUrl = "http://localhost:18080"; // Change if needed
    try {
        std::string url = baseUrl + "/register";
        std::string body = "{\"username\":\"" + username + "\",\"password\":\"" + password + "\"}";
        cpr::Response r = cpr::Post(cpr::Url{url},
                                    cpr::Body{body},
                                    cpr::Header{{"Content-Type", "application/json"}});
        return r.status_code == 200 || r.status_code == 201;
    } catch (...) {
        return false;
    }
}

