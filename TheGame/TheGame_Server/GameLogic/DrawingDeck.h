#pragma once

#include "Card.h"
#include <vector>
#include <random>

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

		static constexpr uint8_t NR_OF_CARDS = 98;
		static constexpr uint8_t START_VALUE = 2;
	};
}

