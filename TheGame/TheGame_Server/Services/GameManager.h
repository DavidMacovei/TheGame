#pragma once
#include <chrono>
#include <map>
#include <mutex>

#include "Game.h"
#include "User.h"

namespace game
{
	struct QueuedPlayer
	{
		std::string username;
		int score;
		std::chrono::steady_clock::time_point joinTime;
	};

	class GameManager
	{
	public:
		GameManager();

		void AddPlayerToQueue(const std::string& username, int score);
		void RemovePlayerFromQueue(const std::string& username);

		bool IsPlayerInQueue(const std::string& username);

		bool TryMatchmaking();

		std::shared_ptr<Game> GetGame(int gameId);
		int GetGameIdForPlayer(const std::string& username);
		std::vector<std::string> GetWaitingList() const;
		int GetSecondsRemaining() const;

	private:
		void CleanupFinishedGames();
		
		std::vector<QueuedPlayer> m_waitingQueue;
		std::map<int, std::shared_ptr<Game>> m_activeGames;
		std::map<std::string, int> m_playerSessions;

		int m_nextGameId = 1;
		mutable std::mutex m_mutex;

		const int MIN_PLAYERS = 2;
		const int MAX_PLAYERS = 5;
		const int MAX_WAIT_SECONDS = 30;
	};
}
