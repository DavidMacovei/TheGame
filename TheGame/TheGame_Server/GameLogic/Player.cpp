#include "Player.h"

Player::Player(std::string username)
{
	this->m_username = username;
}

void Player::AddCardToHand(Card&& c)
{
	m_hand.push_back(std::move(c));
}

const Card& Player::ChooseCardToPlay(int handIndex)
{
	return m_hand[handIndex];
	
}

Card Player::ExtractCard(int handIndex)
{
	Card c = std::move(m_hand[handIndex]);
	m_hand.erase(m_hand.begin() + handIndex);
	return c;
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

