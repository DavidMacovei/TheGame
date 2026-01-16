#pragma once

#include "GameStatus.h"
#include "Player.h"
#include "Board.h"
#include <vector>
#include <mutex>
#include <chrono>

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
		double GetGameDurationInHours() const;

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

		std::chrono::steady_clock::time_point m_startTime;

		mutable std::mutex m_gameMutex;

	private:
		static constexpr int HAND_SIZE_FOR_2_PLAYERS = 8;
		static constexpr int HAND_SIZE_FOR_3_PLAYERS = 7;
		static constexpr int HAND_SIZE_DEFAULT = 6;

		static constexpr int MIN_CARDS_TO_PLAY_NORMAL = 2;
		static constexpr int MIN_CARDS_TO_PLAY_DEPLETED = 1;
	};
}

