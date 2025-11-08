#pragma once

#include "GameStatus.h"
#include "Player.h"
#include "DrawingDeck.h"
#include "PlacingStack.h"
#include <vector>
#include <array>



class Game
{
public:
	Game(const std::vector<std::string>& playerNames);
	bool playCard(int playerIndex, int cardValue, int stackIndex);
	bool endTurn(int playerIndex);
	GameStatus getStatus() const;
	int getCurrentPlayerIndex() const;
	const std::vector<Player>& getPlayers() const;
	const std::array<PlacingStack, 4>& getPlacingStacks() const;
	int getMinimumNumberOfCardsToPlay() const;
	void initializeGame(const std::vector<std::string>& playerNames);
	void nextPlayer();
	void updateGameStatus();
	bool currentPlayerCanPlay() const;

private:
	DrawingDeck drawingDeck;
	std::array<PlacingStack, 4> placingStacks;
	std::vector<Player> players;
	GameStatus status;
	int minimumNumberOfCardsToPlay;
	int cardsPlayedThisTurn;
	int currentPlayerIndex;
};

