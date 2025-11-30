#include "GameStatus.h"
#include <stdexcept>
namespace game {

	GameStatus ToGameStatus(std::string_view str)
	{
		using enum GameStatus;
		if (str == "Active")
		{
			return Active;
		}
		else if (str == "Won")
		{
			return Won;
		}
		else if (str == "Lost")
		{
			return Lost;
		}
		else
			throw std::invalid_argument("Invalid GameStatus string");
	}

	std::string ToString(GameStatus status)
	{
		using enum GameStatus;
		switch (status)
		{
		case Active:
			return "Active";
		case Won:
			return "Won";
		case Lost:
			return "Lost";
		default:
			throw std::invalid_argument("Invalid GameStatus enum value");
		}
	}
}