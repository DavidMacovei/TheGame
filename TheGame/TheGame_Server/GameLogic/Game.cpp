#include "Game.h"
#include <algorithm>

namespace game {

	Game::Game(const std::vector<std::string>& playerNames)
	{
		InitializeGame(playerNames);
	}

	bool Game::PlayCard(uint8_t playerIndex, uint8_t handIndex, uint8_t stackIndex)
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		
		if (playerIndex != m_currentPlayerIndex)
			return false;

		Player& currentPlayer = m_players[playerIndex];
		const Card& candidateCard = currentPlayer.ChooseCardToPlay(handIndex);

		if (m_board.CanPlaceCard(stackIndex, candidateCard))
		{
			Card cardToPlay = currentPlayer.ExtractCard(handIndex);
			m_board.PlaceCard(stackIndex, std::move(cardToPlay));

			m_cardsPlayedThisTurn++;
			UpdateGameStatus();
			return true;
		}
		
		return false;
	}

	bool Game::EndTurn(uint8_t playerIndex)
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		
		if (m_currentPlayerIndex != playerIndex)
			return false;

		if (m_cardsPlayedThisTurn < m_minimumNumberOfCardsToPlay)
			return false;

		Player& currentPlayer = m_players[m_currentPlayerIndex];
		int cardsToDraw = m_cardsPlayedThisTurn;

		for (int i = 0; i < cardsToDraw; i++)
		{
			if (m_board.IsDeckEmpty())
				break;

			Card newCard = m_board.DrawCard();
			currentPlayer.AddCardToHand(std::move(newCard));
		}

		NextPlayer();
		UpdateGameStatus();

		return true;
	}

	bool Game::IsPlayerInGame(const std::string& username) const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		
		for (const auto& player : m_players)
			if (player.GetUsername() == username)
				return true;

		return false;
	}

	GameStatus Game::GetStatus() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		return m_status;
	}

	int Game::GetCurrentPlayerIndex() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		return m_currentPlayerIndex;
	}

	const Board& Game::GetBoard() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		return m_board;
	}

	const std::vector<Player>& Game::GetPlayers() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		return m_players;
	}

	int Game::GetMinimumNumberOfCardsToPlay() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		return m_minimumNumberOfCardsToPlay;
	}

	void Game::InitializeGame(const std::vector<std::string>& playerNames)
	{
		m_board.Reset();
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
				Card newCard = m_board.DrawCard();
				player.AddCardToHand(std::move(newCard));
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

		if (m_board.IsDeckEmpty())
			m_minimumNumberOfCardsToPlay = 1;
		else
			m_minimumNumberOfCardsToPlay = 2;
	}

	void Game::UpdateGameStatus()
	{
		bool allEmpty = std::all_of(m_players.begin(), m_players.end(),
			[](const Player& p) { return p.GetHand().empty(); });
		if (allEmpty && m_board.IsDeckEmpty())
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
		const auto& playerHand = m_players[m_currentPlayerIndex].GetHand();

		for (const auto& card : playerHand)
			if (m_board.CanPlaceCardAnywhere(card))
				return true;

		return false;
	}
}