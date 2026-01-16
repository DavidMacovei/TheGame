#include "User.h"
#include <algorithm>
#include <cmath>

User::User(std::string username, std::string pwd_hash)
	: m_username(std::move(username)), m_password_hash(std::move(pwd_hash))
{
}

int User::GetId() const { return m_id; }
const std::string& User::GetUsername() const { return m_username; }
const std::string& User::GetPasswordHash() const { return m_password_hash; }
int User::GetScore() const { return m_score; }
double User::GetHoursPlayed() const { return m_hours_played; }
int User::GetGamesPlayed() const { return m_games_played; }
int User::GetGamesWon() const { return m_games_won; }
int User::GetTotalCardsLeftOnLoss() const { return m_total_cards_left_on_loss; }

void User::SetUsername(const std::string& username) { m_username = username; }
void User::SetId(int id) { m_id = id; }
void User::SetScore(int s) { m_score = s; }
void User::SetHoursPlayed(double h) { m_hours_played = h; }
void User::SetPasswordHash(const std::string& hash) { m_password_hash = hash; }
void User::SetGamesPlayed(int gp) { m_games_played = gp; }
void User::SetGamesWon(int gw) { m_games_won = gw; }
void User::SetTotalCardsLeftOnLoss(int cards) { m_total_cards_left_on_loss = cards; }

double User::CalculatePerformanceRating() const
{
	if (m_games_played == 0)
		return 3.0; 

	double winRate = static_cast<double>(m_games_won) / m_games_played;
	double winRateScore = winRate * 2.0;

	int gamesLost = m_games_played - m_games_won;
	double lossPerformanceScore = 0.0;
	
	if (gamesLost > 0)
	{
		double avgCardsLeftOnLoss = static_cast<double>(m_total_cards_left_on_loss) / gamesLost;
		
		lossPerformanceScore = std::max(0.0, 2.0 - (avgCardsLeftOnLoss / 49.0));
	}
	else
	{
		
		lossPerformanceScore = 2.0;
	}

	
	double experienceBonus = std::min(1.0, m_games_played / 50.0);

	double rating = 1.0 + winRateScore + lossPerformanceScore + experienceBonus;
	return std::clamp(rating, 1.0, 5.0);
}