#pragma once
#include <string>
#include <nlohmann/json.hpp>

class User 
{
public:
	User() = default;
	User(std::string username, std::string pwd_hash);

	int GetId() const;
	const std::string& GetUsername() const;
	const std::string& GetPasswordHash() const;
	int GetScore() const;
	double GetHoursPlayed() const;
	int GetGamesPlayed() const;
	int GetGamesWon() const;
	int GetTotalCardsLeftOnLoss() const;

	void SetId(int id);
	void SetUsername(const std::string& username);
	void SetPasswordHash(const std::string& hash);
	void SetScore(int s);
	void SetHoursPlayed(double h);
	void SetGamesPlayed(int gp);
	void SetGamesWon(int gw);
	void SetTotalCardsLeftOnLoss(int cards);

	double CalculatePerformanceRating() const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, m_id, m_username, m_score, m_hoursPlayed, m_gamesPlayed, m_gamesWon, m_totalCardsLeftOnLoss)

private:
	int m_id = 0;
	std::string m_username;
	std::string m_passwordHash;
	int m_score = 0;
	double m_hoursPlayed = 0.0;
	int m_gamesPlayed = 0;
	int m_gamesWon = 0;
	int m_totalCardsLeftOnLoss = 0;
};
