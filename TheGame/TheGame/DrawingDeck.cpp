#include "DrawingDeck.h"

const uint8_t nrOfCards = 98;

DrawingDeck::DrawingDeck()
{
	m_cards.resize(nrOfCards);
	for (int i = 0; i < nrOfCards; i++)
		m_cards[i] = i + 2;
}

void DrawingDeck::Shuffle()
{
	std::random_device r_seed;
	std::mt19937 rng(r_seed());
	for (int i = m_cards.size() - 1; i > 0; i--) {
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, i-1);
		int j = dist(rng);
		Card temp;
		temp = m_cards[i];
		m_cards[i] = m_cards[j];
		m_cards[j] = temp;
	}
}

Card DrawingDeck::Draw()
{
	Card local_card(m_cards[m_cards.size() - 1]);
	m_cards.pop_back();
	return local_card;
}

bool DrawingDeck::IsEmpty() const
{
	if (m_cards.size() == 0)
		return true;
	return false;
}

int DrawingDeck::GetLeftoverCardNumber() const
{
	return m_cards.size();
}
