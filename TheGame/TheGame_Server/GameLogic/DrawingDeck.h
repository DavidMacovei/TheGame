#pragma once

#include "Card.h"
#include<vector>
#include<random>

namespace game
{
	class DrawingDeck
	{
	public:
		DrawingDeck();

		void Shuffle();
		Card DrawCard();
		bool IsEmpty() const;
		int GetLeftoverCardNumber() const;

	private:
		std::vector<Card> m_cards;
	};
}

