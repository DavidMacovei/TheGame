#pragma once
#include <cstdint>
#include <compare>

namespace game
{
	class Card
	{
	public:
		Card() = default;
		explicit Card(uint8_t value);

		Card(const Card&) = default;
		Card(Card&&) = default;
		Card& operator=(const Card&) = default;
		Card& operator=(Card&&) = default;
		~Card() = default;

		uint8_t GetValue() const;

		auto operator<=>(const Card& other) const = default;

	private:
		uint8_t m_value;
	};
}
