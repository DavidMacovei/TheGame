#pragma once

#include "GameStatus.h"
#include "Player.h"
#include "Board.h"
#include <vector>
#include <mutex>

namespace game 
{
	class Game
	{
	public:
		Game(const std::vector<std::string>& playerNames);

		bool PlayCard(uint8_t playerIndex, uint8_t handIndex, uint8_t stackIndex);
		bool EndTurn(uint8_t playerIndex);

		bool IsPlayerInGame(const std::string & username) const;

		const Board& GetBoard() const;
		const std::vector<Player>& GetPlayers() const;
		GameStatus GetStatus() const;
		int GetCurrentPlayerIndex() const;
		int GetMinimumNumberOfCardsToPlay() const;

	private:
		void InitializeGame(const std::vector<std::string>& playerNames);
		void NextPlayer();
		void UpdateGameStatus();
		bool CurrentPlayerCanPlay() const;

	private:
		Board m_board;
		std::vector<Player> m_players;

		GameStatus m_status;
		int m_minimumNumberOfCardsToPlay;
		int m_cardsPlayedThisTurn;
		int m_currentPlayerIndex;

		mutable std::mutex m_gameMutex;
	};
}

