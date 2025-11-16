#include "User.h"

User::User() : m_id(0), m_score(0), m_hours_played(0) {}

User::User(const std::string& username, const std::string& pwd_hash)
    : m_id(0), m_username(username), m_password_hash(pwd_hash), m_score(0), m_hours_played(0) {
}

int User::GetId() const { return m_id; }
const std::string& User::GetUsername() const { return m_username; }
const std::string& User::GetPasswordHash() const { return m_password_hash; }
int User::GetScore() const { return m_score; }
int User::GetHoursPlayed() const { return m_hours_played; }

void User::SetScore(int s) { m_score = s; }
void User::SetHoursPlayed(int h) { m_hours_played = h; }
void User::SetPasswordHash(const std::string& hash) { m_password_hash = hash; }

std::string User::ToJson() const {
    std::ostringstream oss;
    oss << "{";
    oss << "\"id\":" << m_id << ",";
    oss << "\"username\":\"" << m_username << "\",";
    oss << "\"score\":" << m_score << ",";
    oss << "\"hours_played\":" << m_hours_played;
    oss << "}";
    return oss.str();
}

User User::FromJson(const std::string& jsonStr) {
    User u;
    size_t pos_id = jsonStr.find("\"id\":");
    size_t pos_username = jsonStr.find("\"username\":\"");
    size_t pos_score = jsonStr.find("\"score\":");
    size_t pos_hours = jsonStr.find("\"hours_played\":");

    if (pos_id != std::string::npos)
        u.m_id = std::stoi(jsonStr.substr(pos_id + 5));
    if (pos_username != std::string::npos) {
        size_t start = pos_username + 12;
        size_t end = jsonStr.find("\"", start);
        u.m_username = jsonStr.substr(start, end - start);
    }
    if (pos_score != std::string::npos)
        u.m_score = std::stoi(jsonStr.substr(pos_score + 8));
    if (pos_hours != std::string::npos)
        u.m_hours_played = std::stoi(jsonStr.substr(pos_hours + 15));

    return u;
}