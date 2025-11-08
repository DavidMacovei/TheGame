#include "Game.h"

Game::Game(const std::vector<std::string>& playerNames) : 
	drawingDeck(), 
	placingStacks{
		PlacingStack(StackType::Ascending),
		PlacingStack(StackType::Ascending),
		PlacingStack(StackType::Descending),
		PlacingStack(StackType::Descending) 
	}
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
	return GameStatus();
}

int Game::getCurrentPlayerIndex() const
{
	return 0;
}

const std::vector<Player>& Game::getPlayers() const
{
	// TODO: insert return statement here
}

const std::array<PlacingStack, 4>& Game::getPlacingStacks() const
{
	// TODO: insert return statement here
}

int Game::getMinimumNumberOfCardsToPlay() const
{
	return 0;
}

void Game::initializeGame(const std::vector<std::string>& playerNames)
{
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

}

void Game::updateGameStatus()
{
}

bool Game::currentPlayerCanPlay() const
{
	return false;
}
