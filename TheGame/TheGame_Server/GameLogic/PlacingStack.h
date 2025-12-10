#pragma once
#include "StackType.h"
#include "Card.h"

class PlacingStack
{
public:
	PlacingStack(StackType type);

	uint8_t GetCurrentValue() const;
	StackType GetType() const;

	bool CanPlace(const Card& card) const;
	void PlaceCard(const Card& card);


private:
	uint8_t m_currentValue;
	StackType m_type;
};

