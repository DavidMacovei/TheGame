#include "Game.h"

Game::Game(const std::vector<std::string>& playerNames)
{
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
