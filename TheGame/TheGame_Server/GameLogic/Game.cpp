#include "Game.h"
#include <algorithm>

namespace game {

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

	bool Game::PlayCard(uint8_t playerIndex, uint8_t handIndex, uint8_t stackIndex)
	{
		if (playerIndex != m_currentPlayerIndex)
		{
			return false;
		}

		if (m_placingStacks[stackIndex].CanPlace(m_players[playerIndex].ChooseCardToPlay(handIndex)))
		{
			m_placingStacks[stackIndex].PlaceCard(m_players[playerIndex].ChooseCardToPlay(handIndex));
			m_players[playerIndex].RemoveCardFromHand(handIndex);

			m_cardsPlayedThisTurn++;

			return true;
		}

		return false;
	}

	bool Game::EndTurn(uint8_t playerIndex)
	{
		if (m_currentPlayerIndex != playerIndex)
			return false;

		if (m_cardsPlayedThisTurn < GetMinimumNumberOfCardsToPlay())
			return false;

		Player& currentPlayer = m_players[m_currentPlayerIndex];
		int cardsToDraw = m_cardsPlayedThisTurn;

		for (int i = 0; i < cardsToDraw; i++)
		{
			if (m_drawingDeck.IsEmpty())
				break;

			Card newCard = m_drawingDeck.DrawCard();
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

	const std::array<PlacingStack, numberOfStacks>& Game::GetPlacingStacks() const
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
				player.AddCardToHand(m_drawingDeck.DrawCard());
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
		if (allEmpty && m_drawingDeck.IsEmpty())
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
		for (int i = 0; i < m_players[m_currentPlayerIndex].GetCardsInHand(); i++) {
			auto playerHand = m_players[m_currentPlayerIndex].GetHand();
			for (int j = 0; j < numberOfStacks; i++) {
				if (m_placingStacks[j].CanPlace(playerHand[i]))
					return true;
			}
		}
		return false;
	}

	std::string Game::GetGameStateAsJson(uint8_t requestingPlayerIndex) const
	{
		GameState gameState;

		gameState.status = ToString(GetStatus());
		gameState.currentPlayer = m_currentPlayerIndex;
		gameState.drawDeckCount = m_drawingDeck.GetLeftoverCardNumber();
		gameState.minCardsToPlay = m_minimumNumberOfCardsToPlay;

		for (size_t i = 0; i < 4; i++)
		{
			StackState stackState;

			stackState.topCardValue = m_placingStacks[i].GetCurrentValue();
			if (m_placingStacks[i].GetType() == StackType::Ascending)
				stackState.isAscending = true;
			else
				stackState.isAscending = false;

			gameState.placingStacks[i] = stackState;
		}

		for (size_t i = 0; i < m_players.size(); i++)
		{
			const auto& serverPlayer = m_players[i];

			PlayerState playerState;
			playerState.cardCount = serverPlayer.GetCardsInHand();
			playerState.username = serverPlayer.GetUsername();

			if (i == requestingPlayerIndex)
			{
				const auto& hand = serverPlayer.GetHand();
				for (const auto& card : hand)
				{
					playerState.hand.push_back(card.GetValue());
				}
			}
			else
			{

			}
			gameState.players.push_back(playerState);
		}
		
		json j = gameState;
		return j.dump();
	}
}