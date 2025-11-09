#include "Player.h"

Player::Player(std::string username)
{
	this->m_username = username;
}

void Player::AddCardToHand(Card c)
{
	m_hand.push_back(c);
}

Card Player::ChooseCardToPlay(int handIndex)
{
	return m_hand[handIndex];
	
}

void Player::RemoveCardFromHand(int handIndex)
{
	m_hand.erase(m_hand.begin() + handIndex);
}

int Player::GetCardsInHand() const
{
	return m_hand.size();
}

const std::vector<Card>& Player::GetHand() const
{
	return m_hand;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

