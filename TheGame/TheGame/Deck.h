#pragma once

#include "Card.h"
#include<vector>
#include<random>

class Deck
{
private:
	std::vector<CardDemo> m_cards;
public:
	Deck();
	void shuffle();
	CardDemo extractCard();
	bool isEmpty() const;
	int getLeftoverCardNumber() const;
};

