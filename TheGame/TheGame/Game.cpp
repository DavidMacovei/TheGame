#include "Game.h"
#include <algorithm>

Game::Game(const std::vector<std::string>& playerNames) :
	m_drawingDeck(),
	m_placingStacks{
		PlacingStack(StackType::Ascending),
		PlacingStack(StackType::Ascending),
		PlacingStack(StackType::Descending),
		PlacingStack(StackType::Descending)
	}
{
	InitializeGame(playerNames);
}

void Game::PlayCard(int playerIndex, int handIndex, int stackIndex)
{
	if (m_placingStacks[stackIndex].CanPlace(m_players[playerIndex].ChooseCardToPlay(handIndex)))
	{
		m_placingStacks[stackIndex].PlaceCard(m_players[playerIndex].ChooseCardToPlay(handIndex));
		m_players[playerIndex].RemoveCardFromHand(handIndex);
	}
}

bool Game::EndTurn(int playerIndex)
{
	if (m_currentPlayerIndex != playerIndex)
		return false;

	if (m_cardsPlayedThisTurn < GetMinimumNumberOfCardsToPlay())
		return false;

	Player& currentPlayer = m_players[m_currentPlayerIndex];
	int cardsToDraw = m_cardsPlayedThisTurn;

	for (int i = 0; i < cardsToDraw; i++)
	{
		if (m_drawingDeck.isEmpty())
			break;

		Card newCard = m_drawingDeck.extractCard();
		currentPlayer.AddCardToHand(newCard);
	}

	NextPlayer();

	UpdateGameStatus();

	return true;
}

GameStatus Game::GetStatus() const
{
	return m_status;
}

int Game::GetCurrentPlayerIndex() const
{
	return m_currentPlayerIndex;
}

const std::vector<Player>& Game::GetPlayers() const
{
	return m_players;
}

const std::array<PlacingStack, 4>& Game::GetPlacingStacks() const
{
	return m_placingStacks;
}

int Game::GetMinimumNumberOfCardsToPlay() const
{
	return m_minimumNumberOfCardsToPlay;
}

void Game::InitializeGame(const std::vector<std::string>& playerNames)
{
	m_players.clear();

	for (const auto& name : playerNames)
	{
		m_players.emplace_back(name);
	}

	int n = (int)m_players.size();
	if (n < 2 || n>5)
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

	for (auto& player : m_players)
	{
		for (int i = 0; i < cardsPerPlayer; ++i)
		{
			player.AddCardToHand(m_drawingDeck.draw());
		}
	}
	m_currentPlayerIndex = 0;
	m_minimumNumberOfCardsToPlay = 2;
	m_cardsPlayedThisTurn = 0;
	m_status = GameStatus::Active;
}

void Game::NextPlayer()
{
	m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
	m_cardsPlayedThisTurn = 0;
}

void Game::UpdateGameStatus()
{
	bool allEmpty = std::all_of(m_players.begin(), m_players.end(),
		[](const Player& p) { return p.GetHand().empty(); });
	if (allEmpty && m_drawingDeck.isEmpty())
	{
		m_status = GameStatus::Won;
		return;
	}

	if (!CurrentPlayerCanPlay())
	{
		m_status = GameStatus::Lost;
		return;
	}
}

bool Game::CurrentPlayerCanPlay() const
{
	return false;
}
