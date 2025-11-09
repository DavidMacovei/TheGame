#pragma once
#include"Card.h"
#include<iostream>
#include<vector>
class Player
{
public:
	Player();
	Player(std::string username);
	void AddCardToHand(Card c);
	Card ChooseCardToPlay(int handIndex);
	void RemoveCardFromHand(int handIndex);
	int GetCardsInHand() const;
	const std::vector<Card>& GetHand() const;
	const std::string& GetUsername() const;

private:
	std::string m_username;
	std::vector<Card>m_hand;
};

