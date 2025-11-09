#pragma once
#include"CardDemo.h"
#include<iostream>
#include<vector>
class Player
{
public:
	Player();
	Player(std::string username);
	void AddCardToHand(CardDemo c);
	CardDemo ChooseCardToPlay(int handIndex);
	int GetCardsInHand() const;
	const std::vector<CardDemo>& GetHand() const;
	const std::string& GetUsername() const;

private:
	std::string m_username;
	std::vector<CardDemo>m_hand;
};

