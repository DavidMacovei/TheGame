#include "DrawingDeck.h"
#include <algorithm>
#include <numeric>
#include <random>

namespace game
{
	const uint8_t NR_OF_CARDS = 98;
	const uint8_t START_VALUE = 2;

	DrawingDeck::DrawingDeck()
	{
		m_cards.resize(NR_OF_CARDS);

		uint8_t currentValue = START_VALUE;
		std::generate(m_cards.begin(), m_cards.end(), [&currentValue]() {
			return Card(currentValue++);
			});
	}

	void DrawingDeck::Shuffle()
	{
		std::random_device rd;
		std::mt19937 g(rd());

		std::ranges::shuffle(m_cards, g);
	}

	Card DrawingDeck::DrawCard()
	{
		Card card = std::move(m_cards.back());
		m_cards.pop_back();
		return card;
	}

	bool DrawingDeck::IsEmpty() const
	{
		return m_cards.empty();
	}

	int DrawingDeck::GetLeftoverCardNumber() const
	{
		return static_cast<int>(m_cards.size());
	}
}
