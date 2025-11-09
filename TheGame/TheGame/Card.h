#pragma once

#include<iostream>

class Card
{
private:
	std::uint8_t m_value;
public:
	Card();
	Card(uint8_t value);
	std::uint8_t GetValue() const;
};
