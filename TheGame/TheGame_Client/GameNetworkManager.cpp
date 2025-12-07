#include "GameNetworkManager.h"

GameNetworkManager::GameNetworkManager(QObject* parent)
    : QObject(parent)
{
    m_gameTimer.setInterval(1500);
    connect(&m_gameTimer, &QTimer::timeout, this, &GameNetworkManager::onPollTimeout);
}

void GameNetworkManager::startGamePolling()
{
    if (!m_gameTimer.isActive()) m_gameTimer.start();
}

void GameNetworkManager::stopGamePolling()
{
    m_gameTimer.stop();
}

void GameNetworkManager::onPollTimeout()
{
    QUrl url(m_baseUrl + "/gameState");
    QNetworkRequest req(url);
    QNetworkReply* reply = m_manager.get(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isObject()) {
            emit gameStateReceived(doc.object());
        }
        });
}

void GameNetworkManager::sendMove(const QString& username, int cardIndex, int stackId)
{
    QUrl url(m_baseUrl + "/makeMove");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["username"] = username;
    obj["cardIndex"] = cardIndex;
    obj["stackId"] = stackId;

    QNetworkReply* reply = m_manager.post(req, QJsonDocument(obj).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        bool success = (reply->error() == QNetworkReply::NoError) && (httpStatus == 200);

        emit moveResult(success, success ? "Move accepted" : "Move rejected by server");
        });
}

void GameNetworkManager::sendChatMessage(const QString& username, const QString& message)
{
    QUrl url(m_baseUrl + "/sendMessage");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["username"] = username;
    obj["message"] = message;

    QNetworkReply* reply = m_manager.post(req, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void GameNetworkManager::endTurn(const QString& username)
{
    QUrl url(m_baseUrl + "/endTurn");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["username"] = username;

    QNetworkReply* reply = m_manager.post(req, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}