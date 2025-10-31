#include "Deck.h"

Deck::Deck()
{
	m_cards.resize(98);
	for (int i = 0; i < 98; i++)
		m_cards[i] = i + 2;
}

void Deck::shuffle()
{
	std::random_device r_seed;
	std::mt19937 rng(r_seed());
	for (int i = m_cards.size() - 1; i > 0; i--) {
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, i-1);
		int j = dist(rng);
		CardDemo temp;
		temp = m_cards[i];
		m_cards[i] = m_cards[j];
		m_cards[j] = temp;
	}
}

CardDemo Deck::extractCard()
{
	CardDemo local_card(m_cards[m_cards.size() - 1]);
	m_cards.pop_back();
	return local_card;
}

bool Deck::isEmpty() const
{
	if (m_cards.size() == 0)
		return true;
	return false;
}

int Deck::getLeftoverCardNumber() const
{
	return m_cards.size();
}
