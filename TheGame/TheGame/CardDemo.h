#pragma once

#include<iostream>

class CardDemo
{
private:
	std::uint8_t m_value;
public:
	CardDemo();
	CardDemo(uint8_t value);
	std::uint8_t getValue() const;
};