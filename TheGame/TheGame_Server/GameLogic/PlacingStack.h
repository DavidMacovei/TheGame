#pragma once
#include "StackType.h"
#include "Card.h"

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
};

