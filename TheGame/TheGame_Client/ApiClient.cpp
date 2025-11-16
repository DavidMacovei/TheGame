#include "ApiClient.h"
#include <sstream>
#include <string>
#include<cpr/cpr.h>

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
