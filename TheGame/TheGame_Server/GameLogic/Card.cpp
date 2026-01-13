#include "Card.h"

Card::Card() : m_value{ 2 } {}

Card::Card(uint8_t value) : m_value(value) {}

uint8_t Card::GetValue() const
{
	return m_value;
}
