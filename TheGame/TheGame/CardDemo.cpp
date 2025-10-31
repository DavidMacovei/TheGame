#include "CardDemo.h"

CardDemo::CardDemo(): m_value(2)
{
}

CardDemo::CardDemo(uint8_t value) :m_value(value)
{ }

uint8_t CardDemo::getValue() const
{
	return m_value;
}
