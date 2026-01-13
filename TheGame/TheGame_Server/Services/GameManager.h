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

		bool IsPlayerInQueue(const std::string& username);

		bool TryMatchmaking();

		void CleanupFinishedGames();

		std::shared_ptr<Game> GetGame(int gameId);
		int GetGameIdForPlayer(const std::string& username);
		std::vector<std::string> GetWaitingList() const;
		int GetSecondsRemaining() const;

	private:
		std::vector<QueuedPlayer> m_waitingQueue;
		std::map<int, std::shared_ptr<Game>> m_activeGames;
		std::map<std::string, int> m_playerSessions;

		int m_nextGameId = 1;
		std::mutex m_mutex;

		const int MIN_PLAYERS = 2;
		const int MAX_PLAYERS = 4;
		const int MAX_WAIT_SECONDS = 30;
	};
}
