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

       

        return r.status_code == 200;
    }
    catch (...) {
        return false;
    }
}
