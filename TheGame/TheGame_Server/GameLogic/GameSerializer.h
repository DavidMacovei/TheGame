#pragma once

#include "Game.h"
#include <string>

namespace game 
{
	std::string SerializeGameState(const Game& gameObj, uint8_t requestingPlayerIndex);
}