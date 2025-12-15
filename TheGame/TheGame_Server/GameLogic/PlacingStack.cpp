#include <cassert>

#include "PlacingStack.h"


constexpr int BACKWARDS_TRICK_OFFSET = 10;

PlacingStack::PlacingStack(StackType type) : m_type{type}
{
	(type == StackType::Ascending) ? (m_currentValue = 1) : (m_currentValue = 100);
}

uint8_t PlacingStack::GetCurrentValue() const
{
	return m_currentValue;
}

StackType PlacingStack::GetType() const
{
	return m_type;
}

bool PlacingStack::CanPlace(const Card& card) const
{
	int cardValue = card.GetValue();

	if (m_type == StackType::Ascending)
	{
		if (cardValue > m_currentValue)
			return true;

		if (cardValue == m_currentValue - BACKWARDS_TRICK_OFFSET)
			return true;
	}
	else
	{
		if (cardValue < m_currentValue)
			return true;

		if (cardValue == m_currentValue + BACKWARDS_TRICK_OFFSET)
			return true;
	}
	return false;
}

void PlacingStack::PlaceCard(Card&& card)
{
	assert(CanPlace(card) && "PRECONDITION FAILED: Cannot place this card!");

	m_currentValue = card.GetValue();
}
