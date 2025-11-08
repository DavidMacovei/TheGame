#pragma once
#include "StackType.h"
#include "Card.h"

class PlacingStack
{
public:
	PlacingStack(StackType type);

	int GetCurrentValue() const;
	StackType GetType() const;

	bool CanPlace(const Card& card) const;
	void PlaceCard(const Card& card);


private:
	int m_currentValue;
	StackType m_type;
};

