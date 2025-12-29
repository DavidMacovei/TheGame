#pragma once

#include<iostream>

class Card
{
private:
	std::uint8_t m_value;
public:
	Card();
	Card(uint8_t value);
    Card(const Card&) = default;
    Card(Card&&) = default;

    Card& operator=(const Card&) = default;
    Card& operator=(Card&&) = default;

    ~Card() = default;

	std::uint8_t GetValue() const;
};
