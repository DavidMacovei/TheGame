#include "ClientApi.h"
#include <crow/json.h>
#include <vector>
#include <string>

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
