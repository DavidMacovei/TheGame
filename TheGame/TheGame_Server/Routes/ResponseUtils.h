#pragma once
#include <crow.h>
#include "GameModels.h"

namespace utils
{
    inline crow::response Error(int code, const std::string& message)
    {
        BasicResponse response;
        response.status = "error";
        response.message = message;
        return crow::response(code, json(response).dump());
    }

    inline crow::response Success(const std::string& message)
    {
        BasicResponse response;
        response.status = "success";
        response.message = message;
        return crow::response(200, json(response).dump());
    }
}
