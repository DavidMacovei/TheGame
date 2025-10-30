#pragma once

#include <cstdint>
#include <string>
#include <string_view>

enum class GameStatus : std::uint8_t
{
	Active,
	Won,
	Lost
};

GameStatus ToGameStatus(std::string_view str);

std::string ToString(GameStatus status);

