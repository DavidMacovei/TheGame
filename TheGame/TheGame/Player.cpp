#include "Player.h"

Player::Player(std::string username)
{
	this->m_username = username;
}

void Player::AddCardToHand(CardDemo c)
{
	m_hand.push_back(c);
}

CardDemo Player::PlayCard(int handIndex)
{
	CardDemo copy = m_hand[handIndex];
	m_hand.erase(m_hand.begin() + handIndex);
	return copy;
}

int Player::GetCardsInHand() const
{
	return m_hand.size();
}

const std::vector<CardDemo>& Player::GetHand() const
{
	return m_hand;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

