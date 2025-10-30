#include "Player.h"

Player::Player(std::string username)
{
	this->username = username;
}

void Player::addCardToHand(CardDemo c)
{
	hand.push_back(c);
}

CardDemo Player::playCard(int handIndex)
{
	CardDemo copy = hand[handIndex];
	hand.erase(hand.begin() + handIndex);
	return copy;
}

int Player::getCardsInHand() const
{
	return hand.size();
}

const std::vector<CardDemo>& Player::getHand() const
{
	return hand;
}

const std::string& Player::getUsername() const
{
	return username;
}

