#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class GameNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit GameNetworkManager(QObject* parent = nullptr);

    void startGamePolling();
    void stopGamePolling();

    void sendMove(const QString& username, int cardIndex, int stackId);
    void sendChatMessage(const QString& username, const QString& message);
    void endTurn(const QString& username);

signals:
    void gameStateReceived(const QJsonObject& gameState);

    void moveResult(bool success, QString message);

private slots:
    void onPollTimeout();

private:
    QNetworkAccessManager m_manager;
    QTimer m_gameTimer;
    QString m_baseUrl = "http://localhost:18080";
};