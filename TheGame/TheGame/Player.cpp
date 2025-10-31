#include "Player.h"

Player::Player(std::string username)
{
	this->m_username = username;
}

void Player::addCardToHand(CardDemo c)
{
	m_hand.push_back(c);
}

CardDemo Player::playCard(int handIndex)
{
	CardDemo copy = m_hand[handIndex];
	m_hand.erase(m_hand.begin() + handIndex);
	return copy;
}

int Player::getCardsInHand() const
{
	return m_hand.size();
}

const std::vector<CardDemo>& Player::getHand() const
{
	return m_hand;
}

const std::string& Player::getUsername() const
{
	return m_username;
}

