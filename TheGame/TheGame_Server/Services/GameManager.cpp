#include "GameManager.h"
#include <algorithm>
#include <ranges>

namespace game
{
	GameManager::GameManager() {}

	void GameManager::AddPlayerToQueue(const std::string& username, int score)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		bool alreadyInQueue = std::ranges::any_of(m_waitingQueue, [&username](const QueuedPlayer& player) {
			return player.username == username;
			});

		if (alreadyInQueue)
			return;

		if (m_playerSessions.contains(username))
			return;

		m_waitingQueue.emplace_back(username, score, std::chrono::steady_clock::now());
	}

	void GameManager::RemovePlayerFromQueue(const std::string& username)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		std::erase_if(m_waitingQueue, [&username](const QueuedPlayer& player) {
			return player.username == username;
			});
	}

	bool GameManager::IsPlayerInQueue(const std::string& username)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		return std::ranges::any_of(m_waitingQueue, [&username](const auto& player) {
			return player.username == username;
			});
	}

	bool GameManager::TryMatchmaking()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		CleanupFinishedGames();

		if (m_waitingQueue.size() < MIN_PLAYERS)
			return false;

		auto now = std::chrono::steady_clock::now();

		std::ranges::sort(m_waitingQueue, {}, &QueuedPlayer::score);

		bool timeoutReached = std::ranges::any_of(m_waitingQueue, [&now](const auto& player) {
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - player.joinTime).count();
			return duration >= MAX_WAIT_SECONDS;
			});

		size_t playersToSelect = 0;

		if (m_waitingQueue.size() >= MAX_PLAYERS)
		{
			playersToSelect = MAX_PLAYERS;
		}
		else if (timeoutReached && m_waitingQueue.size() >= MIN_PLAYERS)
		{
			playersToSelect = std::min((size_t)m_waitingQueue.size(), (size_t)MAX_PLAYERS);
		}

		if (playersToSelect >= MIN_PLAYERS)
		{
			auto selectedUsernamesView = m_waitingQueue
				| std::views::take(playersToSelect)
				| std::views::transform([](const auto& player) {
				return player.username;
					});

			std::vector<std::string> selectedPlayers(selectedUsernamesView.begin(), selectedUsernamesView.end());

			int newId = m_nextGameId++;
			auto newGame = std::make_shared<Game>(selectedPlayers);
			m_activeGames[newId] = newGame;

			std::ranges::for_each(selectedPlayers, [&](const std::string& username) {
				m_playerSessions[username] = newId;
				});

			m_waitingQueue.erase(m_waitingQueue.begin(), m_waitingQueue.begin() + playersToSelect);

			return true;
		}

		return false;
	}

	void GameManager::CleanupFinishedGames()
	{
		for (auto it = m_activeGames.begin(); it != m_activeGames.end(); )
		{
			auto& [id, gamePtr] = *it;

			if (gamePtr->GetStatus() == GameStatus::Won ||
				gamePtr->GetStatus() == GameStatus::Lost)
			{
				std::ranges::for_each(gamePtr->GetPlayers(), [&](const auto& player) {
					m_playerSessions.erase(player.GetUsername());
					});

				it = m_activeGames.erase(it);
			}
			else
				it++;
		}
	}

	std::shared_ptr<Game> GameManager::GetGame(int gameId)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_activeGames.contains(gameId))
			return m_activeGames[gameId];

		return nullptr;
	}

	int GameManager::GetGameIdForPlayer(const std::string& username)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_playerSessions.contains(username))
			return m_playerSessions[username];

		return -1;
	}

	std::vector<std::string> GameManager::GetWaitingList() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		std::vector<std::string> usernames;
		usernames.reserve(m_waitingQueue.size());

		std::ranges::transform(m_waitingQueue, std::back_inserter(usernames), [](const auto& player) {
			return player.username;
			});
	}

	int GameManager::GetSecondsRemaining() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_waitingQueue.empty())
			return MAX_WAIT_SECONDS;

		auto now = std::chrono::steady_clock::now();

		auto oldestPlayerIt = std::ranges::min_element(m_waitingQueue, {}, &QueuedPlayer::joinTime);

		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - oldestPlayerIt->joinTime).count();
		int remaining = MAX_WAIT_SECONDS - (int)elapsed;

		return std::max(0, remaining);
	}
}
