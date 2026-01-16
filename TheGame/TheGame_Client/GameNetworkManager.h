#pragma once

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "ClientApi.h"
#include <memory>

class GameNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit GameNetworkManager(const QString& username, ClientApi* api, QObject* parent = nullptr);

    void startGamePolling();
    void stopGamePolling();

    void sendMove(int cardIndex, int stackId);
    void sendChatMessage(const QString& message);
    void endTurn();
  
    void setPlayerIndex(int playerIndex);
    int getPlayerIndex() const { return m_playerIndex; }

signals:
    void gameStateReceived(const QJsonObject& gameState);
    void moveResult(bool success, QString message);
    void networkError(QString message);

private slots:
    void onPollTimeout();

private:
    QJsonObject convertGameStateToJson(const GameState& state);
    
    ClientApi* m_api;  // Pointer, not unique_ptr - shared with PreGameNetworkManager
    QTimer m_gameTimer;
    QString m_username;
    int m_playerIndex = -1;
};