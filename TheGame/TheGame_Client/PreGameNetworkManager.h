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

    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);

    void joinLobby(const QString& username);
    void leaveLobby(const QString& username);
    void startLobbyPolling();   
    void stopLobbyPolling();
    void setCurrentUsername(const QString& username);

    void fetchProfile(const QString& username);

    ClientApi* getClientApi() { return &m_api; }

signals:
    void loginSuccess(const QString& username);
    void loginFailed(const QString& message);

    void registerSuccess(const QString& username);
    void registerFailed(const QString& message);

    void lobbyWaiting(QStringList waitingPlayers,
        int currentPlayers,
        int neededPlayers,
        int secondsRemaining);

    void lobbyGameStarted(QStringList playersInGame);

    void networkError(const QString& message);

    void profileLoaded(const QString& username,
        int score,
        double hoursPlayed,
        int gamesWon,
        int gamesPlayed);
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
