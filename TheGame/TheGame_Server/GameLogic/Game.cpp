#include "Game.h"
#include <algorithm>

namespace game 
{
	Game::Game(const std::vector<std::string>& playerNames)
	{
		InitializeGame(playerNames);
	}

	bool Game::PlayCard(uint8_t playerIndex, uint8_t handIndex, uint8_t stackIndex)
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		
		if (playerIndex >= m_players.size())
			return false;

		if (playerIndex != m_currentPlayerIndex)
			return false;

		Player& currentPlayer = m_players[playerIndex];

		if(handIndex >= currentPlayer.GetCardsInHand())
			return false;

		const Card& candidateCard = currentPlayer.ChooseCardToPlay(handIndex);

		if (m_board.CanPlaceCard(stackIndex, candidateCard))
		{
			Card cardToPlay = currentPlayer.ExtractCard(handIndex);
			m_board.PlaceCard(stackIndex, std::move(cardToPlay));

			m_cardsPlayedThisTurn++;
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
		{
			if (CurrentPlayerCanPlay())
				return false;
		}

		Player& currentPlayer = m_players[m_currentPlayerIndex];
		int cardsToDraw = m_cardsPlayedThisTurn;

		for (int i = 0; i < cardsToDraw; i++)
		{
			if (m_board.IsDeckEmpty())
				break;

			Card newCard = m_board.DrawCard();
			currentPlayer.AddCardToHand(std::move(newCard));
		}

		UpdateGameStatus();
		NextPlayer();

		return true;
	}

	bool Game::IsPlayerInGame(const std::string& username) const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		
		return std::ranges::any_of(m_players, [&](const Player& p) {
			return p.GetUsername() == username;
			});
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

		m_players.reserve(playerNames.size());
		for (const auto& name : playerNames)
		{
			m_players.emplace_back(name);
		}

		int n = (int)m_players.size();
		if (n < MIN_PLAYERS || n > MAX_PLAYERS)
		{
			throw std::invalid_argument(std::format("Number of players must be between {} and {}.",
				MIN_PLAYERS, MAX_PLAYERS));
		}

		int cardsPerPlayer;
		switch (n)
		{
		case 2:
			cardsPerPlayer = HAND_SIZE_FOR_2_PLAYERS;
			break;
		case 3:
			cardsPerPlayer = HAND_SIZE_FOR_3_PLAYERS;
			break;
		default:
			cardsPerPlayer = HAND_SIZE_DEFAULT;
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

		m_minimumNumberOfCardsToPlay = MIN_CARDS_TO_PLAY_NORMAL;
		m_currentPlayerIndex = 0;
		m_cardsPlayedThisTurn = 0;
		m_status = GameStatus::Active;
		m_startTime = std::chrono::steady_clock::now();
	}

	void Game::NextPlayer()
	{
		m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
		m_cardsPlayedThisTurn = 0;

		if (m_board.IsDeckEmpty())
			m_minimumNumberOfCardsToPlay = MIN_CARDS_TO_PLAY_DEPLETED;
		else
			m_minimumNumberOfCardsToPlay = MIN_CARDS_TO_PLAY_NORMAL;
	}

	void Game::UpdateGameStatus()
	{
		bool allEmpty = std::ranges::all_of(m_players,
			[](const Player& p) { 
				return p.GetHand().empty(); 
			});

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

		return std::ranges::any_of(playerHand, [&](const Card& c) {
			return m_board.CanPlaceCardAnywhere(c);
			});
	}

	double Game::GetGameDurationInHours() const
	{
		std::lock_guard<std::mutex> lock(m_gameMutex);
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - m_startTime);
		return duration.count() / 3600.0;
	}
}