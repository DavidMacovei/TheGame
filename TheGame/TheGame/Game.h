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
	bool PlayCard(int playerIndex, int cardValue, int stackIndex);
	bool EndTurn(int playerIndex);
	GameStatus GetStatus() const;
	int GetCurrentPlayerIndex() const;
	const std::vector<Player>& GetPlayers() const;
	const std::array<PlacingStack, 4>& GetPlacingStacks() const;
	int GetMinimumNumberOfCardsToPlay() const;
	void InitializeGame(const std::vector<std::string>& playerNames);
	void NextPlayer();
	void UpdateGameStatus();
	bool CurrentPlayerCanPlay() const;

private:
	DrawingDeck m_drawingDeck;
	std::array<PlacingStack, 4> m_placingStacks;
	std::vector<Player> m_players;
	GameStatus m_status;
	int m_minimumNumberOfCardsToPlay;
	int m_cardsPlayedThisTurn;
	int m_currentPlayerIndex;
};

