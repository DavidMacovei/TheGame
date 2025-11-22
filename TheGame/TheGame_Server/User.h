#pragma once
#include <string>
#include <sstream>

class User {
public:
    User();
    User(const std::string& username, const std::string& pwd_hash);

    int GetId() const;
    const std::string& GetUsername() const;
    const std::string& GetPasswordHash() const;
    int GetScore() const;
    double GetHoursPlayed() const;

    void SetId(int id);
    void SetUsername(const std::string& username);
    void SetScore(int s);
    void SetHoursPlayed(double h);
    void SetPasswordHash(const std::string& hash);

    std::string ToJson() const;
    static User FromJson(const std::string& jsonStr);

private:
    int m_id;
    std::string m_username;
    std::string m_password_hash;
    int m_score;
    int m_hours_played;
};
