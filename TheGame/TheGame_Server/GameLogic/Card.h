#pragma once
#include<cstdint>

namespace game
{
	class Card
	{
	public:
		Card();
		explicit Card(uint8_t value);

		Card(const Card&) = default;
		Card(Card&&) = default;
		Card& operator=(const Card&) = default;
		Card& operator=(Card&&) = default;
		~Card() = default;

		std::uint8_t GetValue() const;

		auto operator<=>(const Card& other) const = default;

	private:
		std::uint8_t m_value;
	};
}
