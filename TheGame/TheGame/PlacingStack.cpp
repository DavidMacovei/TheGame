#include "PlacingStack.h"

PlacingStack::PlacingStack(StackType type)
{
}

int PlacingStack::getCurrentValue() const
{
	return 0;
}

StackType PlacingStack::getType() const
{
	return StackType();
}

bool PlacingStack::canPlace(const CardDemo& card) const
{
	return false;
}

void PlacingStack::placeCard(const CardDemo& card)
{
}
