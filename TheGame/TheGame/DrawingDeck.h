#pragma once

#include "Card.h"
#include<vector>
#include<random>

class DrawingDeck
{
private:
	std::vector<Card> m_cards;
public:
	DrawingDeck();
	void Shuffle();
	Card DrawCard();
	bool IsEmpty() const;
	int GetLeftoverCardNumber() const;
};

