#include "User.h"

User::User(std::string username, std::string pwd_hash)
	: m_username(std::move(username)), m_password_hash(std::move(pwd_hash))
{
}

int User::GetId() const { return m_id; }
const std::string& User::GetUsername() const { return m_username; }
const std::string& User::GetPasswordHash() const { return m_password_hash; }
int User::GetScore() const { return m_score; }
double User::GetHoursPlayed() const { return m_hours_played; }

void User::SetUsername(const std::string& username) { m_username = username; }
void User::SetId(int id) { m_id = id; }
void User::SetScore(int s) { m_score = s; }
void User::SetHoursPlayed(double h) { m_hours_played = h; }
void User::SetPasswordHash(const std::string& hash) { m_password_hash = hash; }