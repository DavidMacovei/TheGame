#include "User.h"
#include <algorithm>
#include <cmath>

User::User(std::string username, std::string pwd_hash)
	: m_username(std::move(username)), m_passwordHash(std::move(pwd_hash))
{
}

int User::GetId() const { return m_id; }
const std::string& User::GetUsername() const { return m_username; }
const std::string& User::GetPasswordHash() const { return m_passwordHash; }
int User::GetScore() const { return m_score; }
double User::GetHoursPlayed() const { return m_hoursPlayed; }
int User::GetGamesPlayed() const { return m_gamesPlayed; }
int User::GetGamesWon() const { return m_gamesWon; }
int User::GetTotalCardsLeftOnLoss() const { return m_totalCardsLeftOnLoss; }

void User::SetUsername(const std::string& username) { m_username = username; }
void User::SetId(int id) { m_id = id; }
void User::SetScore(int s) { m_score = s; }
void User::SetHoursPlayed(double h) { m_hoursPlayed = h; }
void User::SetPasswordHash(const std::string& hash) { m_passwordHash = hash; }
void User::SetGamesPlayed(int gp) { m_gamesPlayed = gp; }
void User::SetGamesWon(int gw) { m_gamesWon = gw; }
void User::SetTotalCardsLeftOnLoss(int cards) { m_totalCardsLeftOnLoss = cards; }

double User::CalculatePerformanceRating() const
{
	if (m_gamesPlayed == 0)
		return 3.0; 

	double winRate = static_cast<double>(m_gamesWon) / m_gamesPlayed;
	double winRateScore = winRate * 2.0;

	int gamesLost = m_gamesPlayed - m_gamesWon;
	double lossPerformanceScore = 0.0;
	
	if (gamesLost > 0)
	{
		double avgCardsLeftOnLoss = static_cast<double>(m_totalCardsLeftOnLoss) / gamesLost;
		
		lossPerformanceScore = std::max(0.0, 2.0 - (avgCardsLeftOnLoss / 49.0));
	}
	else
	{
		
		lossPerformanceScore = 2.0;
	}

	
	double experienceBonus = std::min(1.0, m_gamesPlayed / 50.0);

	double rating = 1.0 + winRateScore + lossPerformanceScore + experienceBonus;
	return std::clamp(rating, 1.0, 5.0);
}