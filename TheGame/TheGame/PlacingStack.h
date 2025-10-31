#pragma once
#include "StackType.h"

#include "CardDemo.h"

class PlacingStack
{
public:
	PlacingStack(StackType type);

	int getCurrentValue() const;
	StackType getType() const;

	bool canPlace(const CardDemo& card) const;
	void placeCard(const CardDemo& card);


private:
	int m_currentValue;
	StackType m_type;
};

