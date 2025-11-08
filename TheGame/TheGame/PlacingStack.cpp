#include <cassert>

#include "PlacingStack.h"


constexpr int BACKWARDS_TRICK_OFFSET = 10;

PlacingStack::PlacingStack(StackType type) : m_type{type}
{
	(type == StackType::ASCENDING) ? (m_currentValue = 1) : (m_currentValue = 100);
}

int PlacingStack::GetCurrentValue() const
{
	return m_currentValue;
}

StackType PlacingStack::GetType() const
{
	return m_type;
}

bool PlacingStack::CanPlace(const CardDemo& card) const
{
	int cardValue = card.getValue();

	if (m_type == StackType::ASCENDING)
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

void PlacingStack::PlaceCard(const CardDemo& card)
{
	assert(CanPlace(card) && "PRECONDITION FAILED: Cannot place this card!");

	m_currentValue = card.getValue();
}
