#include "GameManager.h"
#include <algorithm>

namespace game
{
	GameManager::GameManager() {}

	void GameManager::AddPlayerToQueue(const std::string& username, int score)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		
		for (const auto& player : m_waitingQueue)
			if (player.username == username)
				return;

		if (m_playerSessions.find(username) != m_playerSessions.end())
			return;

		QueuedPlayer qp{ username, score, std::chrono::steady_clock::now() };

		m_waitingQueue.push_back(qp);
	}

	bool GameManager::IsPlayerInQueue(const std::string& username)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (const auto& player : m_waitingQueue)
			if (player.username == username)
				return true;

		return false;
	}

	bool GameManager::TryMatchmaking()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_waitingQueue.size() < MIN_PLAYERS)
			return false;

		auto now = std::chrono::steady_clock::now();
		std::vector<std::string> selectedPlayers;

		std::sort(m_waitingQueue.begin(), m_waitingQueue.end(), [](const QueuedPlayer& a, const QueuedPlayer& b) {
			return a.score < b.score;
			});

		bool timeoutReached = false;

		for (const auto& player : m_waitingQueue)
		{
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - player.joinTime).count();
			if (duration >= MAX_WAIT_SECONDS)
			{
				timeoutReached = true;
				break;
			}
		}

		int playersToSelect = 0;

		if (m_waitingQueue.size() >= MAX_PLAYERS)
		{
			playersToSelect = MAX_PLAYERS;
		}
		else if (timeoutReached && m_waitingQueue.size() >= MIN_PLAYERS)
		{
			playersToSelect = std::min((int)m_waitingQueue.size(), MAX_PLAYERS);
		}

		if (playersToSelect >= MIN_PLAYERS)
		{
			for (int i = 0; i < playersToSelect; i++)
				selectedPlayers.push_back(m_waitingQueue[i].username);

			int newId = m_nextGameId++;
			auto newGame = std::make_shared<Game>(selectedPlayers);
			m_activeGames[newId] = newGame;

			for (const auto& username : selectedPlayers)
				m_playerSessions[username] = newId;

			m_waitingQueue.erase(m_waitingQueue.begin(), m_waitingQueue.begin() + playersToSelect);

			return true;
		}

		return false;
	}

	void GameManager::CleanupFinishedGames()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_activeGames.begin(); it != m_activeGames.end(); )
		{
			if (it->second->GetStatus() == GameStatus::Won ||
				it->second->GetStatus() == GameStatus::Lost)
			{
				for (const auto& player : it->second->GetPlayers())
					m_playerSessions.erase(player.GetUsername());
				it = m_activeGames.erase(it);
			}
			else
				it++;
		}
	}

	std::shared_ptr<Game> GameManager::GetGame(int gameId)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_activeGames.find(gameId) != m_activeGames.end())
			return m_activeGames[gameId];

		return nullptr;
	}

	int GameManager::GetGameIdForPlayer(const std::string& username)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_playerSessions.find(username) != m_playerSessions.end())
			return m_playerSessions[username];

		return -1;
	}

	std::vector<std::string> GameManager::GetWaitingList() const
	{
		std::vector<std::string> usernames;
		for (const auto& player : m_waitingQueue)
			usernames.push_back(player.username);
		return usernames;
	}

	int GameManager::GetSecondsRemaining() const
	{
		if (m_waitingQueue.empty())
			return MAX_WAIT_SECONDS;

		auto now = std::chrono::steady_clock::now();
		auto oldestJoinTime = m_waitingQueue[0].joinTime;

		for (const auto& player : m_waitingQueue)
		{
			if (player.joinTime < oldestJoinTime)
				oldestJoinTime = player.joinTime;
		}

		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - oldestJoinTime).count();
		int remaining = MAX_WAIT_SECONDS - (int)elapsed;

		return std::max(0, remaining);
	}
}
