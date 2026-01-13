#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QStringList>
#include <QtConcurrent>
#include "ClientApi.h"

class PreGameNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit PreGameNetworkManager(QObject* parent = nullptr);

    // Auth
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);

    // Lobby
    void joinLobby(const QString& username);
    void leaveLobby(const QString& username);
    void startLobbyPolling();   
    void stopLobbyPolling();

    void fetchProfile(const QString& username);

signals:
    // Auth
    void loginSuccess(const QString& username);
    void loginFailed(const QString& message);

    void registerSuccess(const QString& username);
    void registerFailed(const QString& message);

    // Lobby
    void lobbyWaiting(QStringList waitingPlayers,
        int currentPlayers,
        int neededPlayers,
        int secondsRemaining);

    void lobbyGameStarted(QStringList playersInGame);

    void networkError(const QString& message);

    // Profile
    void profileLoaded(const QString& username,
        int score,
        double hoursPlayed);
    void profileError(const QString& message);

private slots:
    void onLobbyPollTimeout();

private:
    QNetworkAccessManager m_manager;
    QTimer m_lobbyTimer;
    QString m_baseUrl = "http://localhost:18080";
    QString m_currentUsername;

    ClientApi m_api;
};
