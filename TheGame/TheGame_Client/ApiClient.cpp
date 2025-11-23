#include "ApiClient.h"
#include <sstream>
#include <string>
#include <cpr/cpr.h>
#include <cctype>

static bool parseBoolField(const std::string& body, const std::string& fieldName, bool& outValue) {
    std::string key = std::string("\"") + fieldName + "\"";
    size_t pos = body.find(key);
    if (pos == std::string::npos) return false;
    pos = body.find(':', pos + key.size());
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) pos++;
    if (pos + 4 <= body.size() && body.compare(pos, 4, "true") == 0) { outValue = true; return true; }
    if (pos + 5 <= body.size() && body.compare(pos, 5, "false") == 0) { outValue = false; return true; }
    return false;
}

static bool parseStringField(const std::string& body, const std::string& fieldName, std::string& outValue) {
    std::string key = std::string("\"") + fieldName + "\"";
    size_t pos = body.find(key);
    if (pos == std::string::npos) return false;
    size_t colon = body.find(':', pos + key.size());
    if (colon == std::string::npos) return false;
    size_t firstQuote = body.find('"', colon);
    if (firstQuote == std::string::npos) return false;
    size_t secondQuote = body.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) return false;
    outValue = body.substr(firstQuote + 1, secondQuote - firstQuote - 1);
    return true;
}

static bool parseIntField(const std::string& body, const std::string& fieldName, int& outValue) {
    std::string key = std::string("\"") + fieldName + "\"";
    size_t pos = body.find(key);
    if (pos == std::string::npos) return false;
    size_t colon = body.find(':', pos + key.size());
    if (colon == std::string::npos) return false;
    size_t p = colon + 1;
    while (p < body.size() && std::isspace(static_cast<unsigned char>(body[p]))) p++;
    size_t q = p;
    if (q < body.size() && (body[q] == '-' || (body[q] >= '0' && body[q] <= '9'))) q++; else return false;
    while (q < body.size() && (body[q] >= '0' && body[q] <= '9')) q++;
    try {
        outValue = std::stoi(body.substr(p, q - p));
        return true;
    } catch (...) {
        return false;
    }
}

static void parseIntArrayFromRange(const std::string& s, size_t startInclusive, size_t endExclusive, std::vector<int>& out) {
    size_t p = startInclusive;
    while (p < endExclusive) {
        // skip non-digit and non-minus
        while (p < endExclusive && !(s[p] == '-' || (s[p] >= '0' && s[p] <= '9'))) p++;
        if (p >= endExclusive) break;
        size_t q = p + 1;
        while (q < endExclusive && (s[q] >= '0' && s[q] <= '9')) q++;
        try {
            out.push_back(std::stoi(s.substr(p, q - p)));
        } catch (...) {
            // ignore parse errors
        }
        p = q;
    }
}

ApiClient::ApiClient(const std::string& baseUrl)
    : baseUrl(baseUrl)
{
}

bool ApiClient::login(const std::string& user)
{
    try {
        std::ostringstream oss;
        oss << "{\"username\":\"" << user << "\"}";
        std::string url = baseUrl + "/login";

        cpr::Response r = cpr::Post(cpr::Url{url},
                                    cpr::Body{oss.str()},
                                    cpr::Header{{"Content-Type", "application/json"}});

        const std::string& body = r.text;

        // If response contains JSON, try to interpret common fields.
        bool bval = false;
        if (parseBoolField(body, "success", bval)) return bval;

        std::string sval;
        if (parseStringField(body, "status", sval)) {
            if (sval == "ok" || sval == "success") return true;
            return false;
        }
        if (parseStringField(body, "result", sval)) {
            if (sval == "ok" || sval == "success") return true;
            return false;
        }

        // If error/message present, treat as failure
        if (parseStringField(body, "error", sval) || parseStringField(body, "message", sval)) return false;

        // fallback to HTTP status

        return r.status_code == 200;
    }
    catch (...) {
        return false;
    }
}

GameState ApiClient::getGameState()
{
    GameState gs;
    try {
        std::string url = baseUrl + "/gameState";
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code != 200) return gs;
        const std::string& body = r.text;

        // parse basic fields
        parseStringField(body, "status", gs.status);
        parseIntField(body, "currentPlayer", gs.currentPlayer);
        parseIntField(body, "drawDeckCount", gs.drawDeckCount);
        parseIntField(body, "minCardsToPlay", gs.minCardsToPlay);

        // parse players: expect "players": [ {"username":"u","hand":[1,2]}, ... ]
        size_t posPlayers = body.find("\"players\"");
        if (posPlayers != std::string::npos) {
            size_t start = body.find('[', posPlayers);
            size_t end = body.find(']', start);
            if (start != std::string::npos && end != std::string::npos) {
                size_t i = start + 1;
                while (i < end) {
                    // find next object
                    size_t objStart = body.find('{', i);
                    if (objStart == std::string::npos || objStart >= end) break;
                    size_t objEnd = body.find('}', objStart);
                    if (objEnd == std::string::npos || objEnd >= end) break;
                    std::string obj = body.substr(objStart, objEnd - objStart + 1);
                    PlayerState ps;
                    parseStringField(obj, "username", ps.username);
                    // find hand array inside object
                    size_t posHand = obj.find("\"hand\"");
                    if (posHand != std::string::npos) {
                        size_t hb = obj.find('[', posHand);
                        size_t he = obj.find(']', hb);
                        if (hb != std::string::npos && he != std::string::npos) {
                            parseIntArrayFromRange(obj, hb + 1, he, ps.hand);
                        }
                    }
                    gs.players.push_back(ps);
                    i = objEnd + 1;
                }
            }
        }

        // parse stacks: expect "stacks": [[1,2],[3],[]]
        size_t posStacks = body.find("\"stacks\"");
        if (posStacks != std::string::npos) {
            size_t start = body.find('[', posStacks);
            size_t end = body.find(']', start);
            if (start != std::string::npos && end != std::string::npos) {
                size_t i = start + 1;
                while (i < end) {
                    size_t a = body.find('[', i);
                    if (a == std::string::npos || a >= end) break;
                    size_t b = body.find(']', a);
                    if (b == std::string::npos || b >= end) break;
                    std::vector<int> stackVals;
                    parseIntArrayFromRange(body, a + 1, b, stackVals);
                    gs.stacks.push_back(stackVals);
                    i = b + 1;
                }
            }
        }

        return gs;
    }
    catch (...) {
        return gs;
    }
}
