#pragma once

#include<iostream>
#include<vector>
#include"Card.h"

class Player
{
public:
	Player() = default;
	Player(std::string username);

	void AddCardToHand(Card&& c);
	const Card& ChooseCardToPlay(int handIndex);
	Card ExtractCard(int handIndex);

	int GetCardsInHand() const;
	const std::vector<Card>& GetHand() const;
	const std::string& GetUsername() const;

private:
	std::string m_username;
	std::vector<Card> m_hand;
};

