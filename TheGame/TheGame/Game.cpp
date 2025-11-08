#include "Game.h"

#include <algorithm>

Game::Game(const std::vector<std::string>& playerNames)
{
	initializeGame(playerNames);
}

bool Game::playCard(int playerIndex, int cardValue, int stackIndex)
{
	return false;
}

bool Game::endTurn(int playerIndex)
{
	return false;
}

GameStatus Game::getStatus() const
{
	return status;
}

int Game::getCurrentPlayerIndex() const
{
	return currentPlayerIndex;
}

const std::vector<Player>& Game::getPlayers() const
{
	return players;
}

const std::array<PlacingStack, 4>& Game::getPlacingStacks() const
{
	return placingStacks;
}

int Game::getMinimumNumberOfCardsToPlay() const
{
	return minimumNumberOfCardsToPlay;
}

void Game::initializeGame(const std::vector<std::string>& playerNames)
{
	drawingDeck = DrawingDeck();

	placingStacks = {
		PlacingStack(StackType::ASCENDING),
		PlacingStack(StackType::ASCENDING),
		PlacingStack(StackType::DESCENDING),
		PlacingStack(StackType::DESCENDING) 
	};

	players.clear();
	for (const auto& name : playerNames)
	{
		players.emplace_back(name);
	}

	int n = (int)players.size();
	if(n<2 || n>5)
	{
		throw std::invalid_argument("Number of players must be between 2 and 5.");
	}

	int cardsPerPlayer;
	switch (n)
	{
	case 2:
		cardsPerPlayer = 8;
		break;
	case 3:
		cardsPerPlayer = 7;
		break;
	default:
		cardsPerPlayer = 6;
		break;
	}

	for (auto& player : players)
	{
		for (int i = 0; i < cardsPerPlayer; ++i)
		{
			player.addCardToHand(drawingDeck.draw());
		}
	}
	currentPlayerIndex = 0;
	minimumNumberOfCardsToPlay = 2;
	cardsPlayedThisTurn = 0;
	status = GameStatus::Active;
}

void Game::nextPlayer()
{
	currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
	cardsPlayedThisTurn = 0;
}

void Game::updateGameStatus()
{
	bool allEmpty = std::all_of(players.begin(), players.end(),
		[](const Player& p) { return p.getHand().empty(); });
	if (allEmpty && drawingDeck.isEmpty())
	{
		status = GameStatus::Won;
		return;
	}

	if (!currentPlayerCanPlay())
	{
		status = GameStatus::Lost;
		return;
	}
}

bool Game::currentPlayerCanPlay() const
{
	return false;
}
