#pragma once
#include"CardDemo.h"
#include<iostream>
#include<vector>
class Player
{
public:
	Player(std::string username);
	void addCardToHand(CardDemo c);
	CardDemo playCard(int handIndex);
	int getCardsInHand() const;
	const std::vector<CardDemo>& getHand() const;
	const std::string& getUsername() const;

private:
	std::string m_username;
	std::vector<CardDemo>m_hand;
};

