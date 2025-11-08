#pragma once
#include "StackType.h"
#include "CardDemo.h"

class PlacingStack
{
public:
	PlacingStack(StackType type);

	int GetCurrentValue() const;
	StackType GetType() const;

	bool CanPlace(const CardDemo& card) const;
	void PlaceCard(const CardDemo& card);


private:
	int m_currentValue;
	StackType m_type;
};

