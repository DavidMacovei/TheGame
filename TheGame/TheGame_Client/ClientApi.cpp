#include "ClientApi.h"

LobbyStatus ClientApi::getLobbyStatus() {
    LobbyStatus status;
    try {
        std::string url = baseUrl + "/lobbyStatus";
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code != 200) return status;
        auto obj = crow::json::load(r.text);
        if (!obj) return status;

        if (obj.has("gameStarted")) status.gameStarted = obj["gameStarted"].b();
        if (obj.has("playerCount")) status.playerCount = obj["playerCount"].i();
        if (obj.has("maxPlayers")) status.maxPlayers = obj["maxPlayers"].i();
        if (obj.has("playerNames")) {
            auto arr = obj["playerNames"];
            for (size_t i = 0; i < arr.size(); ++i) {
                status.playerNames.push_back(arr[i].s());
            }
        }
    } catch (...) {
        // return default status
    }
    return status;
}



GameState ClientApi::getGameState() {
    GameState gs;
    try {
        std::string url = baseUrl + "/gameState";
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code != 200) return gs;
        auto obj = crow::json::load(r.text);
        if (!obj) return gs;

        // Parse basic fields
        if (obj.has("status")) gs.status = obj["status"].s();
        if (obj.has("currentPlayer")) gs.currentPlayer = obj["currentPlayer"].i();
        if (obj.has("drawDeckCount")) gs.drawDeckCount = obj["drawDeckCount"].i();
        if (obj.has("minCardsToPlay")) gs.minCardsToPlay = obj["minCardsToPlay"].i();

        // Parse players
        if (obj.has("players")) {
            auto arr = obj["players"];
            for (size_t i = 0; i < arr.size(); ++i) {
                PlayerState ps;
                auto pobj = arr[i];
                if (pobj.has("username")) ps.username = pobj["username"].s();
                if (pobj.has("hand")) {
                    auto handArr = pobj["hand"];
                    for (size_t j = 0; j < handArr.size(); ++j) {
                        ps.hand.push_back(handArr[j].i());
                    }
                }
                gs.players.push_back(ps);
            }
        }

        // Parse stacks
        if (obj.has("stacks")) {
            auto stacksArr = obj["stacks"];
            for (size_t i = 0; i < stacksArr.size(); ++i) {
                std::vector<int> stack;
                auto stackArr = stacksArr[i];
                for (size_t j = 0; j < stackArr.size(); ++j) {
                    stack.push_back(stackArr[j].i());
                }
                gs.stacks.push_back(stack);
            }
        }
    } catch (...) {
        // return default gs
    }
    return gs;
}

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

