#pragma once

#include "Game.h"
#include <string>

namespace game 
{
	std::string SerializeGameState(const Game& gameObj, const std::string& requestingUsername);
}