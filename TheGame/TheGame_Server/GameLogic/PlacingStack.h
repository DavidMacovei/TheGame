#pragma once
#include "StackType.h"
#include "Card.h"

namespace game
{
	class PlacingStack
	{
	public:
		PlacingStack() = default;
		PlacingStack(StackType type);

		bool CanPlace(const Card& card) const;
		void PlaceCard(Card&& card);

		uint8_t GetCurrentValue() const;
		StackType GetType() const;

	private:
		uint8_t m_currentValue;
		StackType m_type;

		static constexpr uint8_t BACKWARDS_TRICK_OFFSET = 10;
		static constexpr uint8_t START_VALUE_ASCENDING = 1;
		static constexpr uint8_t START_VALUE_DESCENDING = 100;
	};
}

