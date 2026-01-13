#include <cassert>
#include <cmath>
#include <format>
#include <stdexcept>
#include "PlacingStack.h"

namespace game
{
	const int BACKWARDS_TRICK_OFFSET = 10;

	PlacingStack::PlacingStack(StackType type) : m_type{ type }
	{
		m_currentValue = (type == StackType::Ascending) ? 1 : 100;
	}

	bool PlacingStack::CanPlace(const Card& card) const
	{
		int cardValue = card.GetValue();
		int diff = cardValue - m_currentValue;

		bool isBackwardsMove = std::abs(diff) == BACKWARDS_TRICK_OFFSET;
		if (isBackwardsMove)
			return true;

		if (m_type == StackType::Ascending)
			return cardValue > m_currentValue;
		else
			return cardValue < m_currentValue;
	}

	void PlacingStack::PlaceCard(Card&& card)
	{
		if (!CanPlace(card))
			throw std::logic_error(
				std::format("Invalid move: Cannot place card {} over {}.", card.GetValue(), m_currentValue)
			);

		m_currentValue = card.GetValue();
	}

	uint8_t PlacingStack::GetCurrentValue() const
	{
		return m_currentValue;
	}

	StackType PlacingStack::GetType() const
	{
		return m_type;
	}
}
