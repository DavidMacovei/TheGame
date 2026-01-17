#include "PreGameNetworkManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>

PreGameNetworkManager::PreGameNetworkManager(QObject* parent)
    : QObject(parent)
    , m_api(m_baseUrl.toStdString())
{
    m_lobbyTimer.setInterval(1000); // 1s
    connect(&m_lobbyTimer, &QTimer::timeout,
        this, &PreGameNetworkManager::onLobbyPollTimeout);
}

void PreGameNetworkManager::login(const QString& username,
    const QString& password)
{
    QtConcurrent::run([this, username, password]() {

        BasicResponse resp = m_api.Login(
            username.toStdString(),
            password.toStdString());

        QMetaObject::invokeMethod(this, [=]() {

            if (resp.status == "success") {
                emit loginSuccess(username);
            }
            else {
                emit loginFailed(
                    QString::fromStdString(resp.message));
            }

            }, Qt::QueuedConnection);
        });
}

void PreGameNetworkManager::registerUser(const QString& username,
    const QString& password)
{
    QtConcurrent::run([this, username, password]() {
        BasicResponse resp = m_api.RegisterUser(
            username.toStdString(),
            password.toStdString());

        QMetaObject::invokeMethod(this, [=]() {
            if (resp.status == "success") {
                emit registerSuccess(username);
            }
            else {
                emit registerFailed(QString::fromStdString(resp.message));
            }
            }, Qt::QueuedConnection);
        });
}

void PreGameNetworkManager::joinLobby(const QString& username)
{
    m_currentUsername = username;
    
    QtConcurrent::run([this, username]() {
        BasicResponse joinResp = m_api.JoinLobby(username.toStdString());

        if (joinResp.status != "success") {
            QMetaObject::invokeMethod(this, [=]() {
                emit networkError(QString::fromStdString(joinResp.message));
            }, Qt::QueuedConnection);
            return;
        }

        UserStatusResponse statusResp = m_api.GetUserStatus(username.toStdString());

        QMetaObject::invokeMethod(this, [=]() {
            if (statusResp.status == "running" && statusResp.gameId != -1) {
                m_api.SetActiveGame(statusResp.gameId);
                
                QStringList players;
                for (const auto& player : statusResp.waitingList) {
                    players << QString::fromStdString(player);
                }
                emit lobbyGameStarted(players);
            }
            else if (statusResp.status == "waiting") {
                QStringList waiting;
                for (const auto& player : statusResp.waitingList) {
                    waiting << QString::fromStdString(player);
                }
                emit lobbyWaiting(waiting, statusResp.playersInQueue, 5, statusResp.secondsRemaining);
            }
            else {
                emit networkError("Unknown lobby status: " + QString::fromStdString(statusResp.status));
            }
        }, Qt::QueuedConnection);
    });
}

void PreGameNetworkManager::leaveLobby(const QString& username)
{
    QtConcurrent::run([this, username]() {
        m_api.LeaveLobby(username.toStdString());
    });
}

void PreGameNetworkManager::startLobbyPolling()
{
    if (!m_lobbyTimer.isActive())
        m_lobbyTimer.start();
}

void PreGameNetworkManager::stopLobbyPolling()
{
    m_lobbyTimer.stop();
}

void PreGameNetworkManager::setCurrentUsername(const QString& username)
{
    m_currentUsername = username;
}

void PreGameNetworkManager::onLobbyPollTimeout()
{
    if (m_currentUsername.isEmpty())
        return;

    QtConcurrent::run([this]() {
        UserStatusResponse statusResp = m_api.GetUserStatus(m_currentUsername.toStdString());

        QMetaObject::invokeMethod(this, [=]() {
            if (statusResp.status == "error") {
                emit networkError(QString::fromStdString(statusResp.message));
                return;
            }

            if (statusResp.status == "running" && statusResp.gameId != -1) {
                m_api.SetActiveGame(statusResp.gameId);
                
                QStringList players;
                for (const auto& player : statusResp.waitingList) {
                    players << QString::fromStdString(player);
                }
                emit lobbyGameStarted(players);
            }
            else if (statusResp.status == "waiting") {
                QStringList waiting;
                for (const auto& player : statusResp.waitingList) {
                    waiting << QString::fromStdString(player);
                }
                emit lobbyWaiting(waiting, statusResp.playersInQueue, 5, statusResp.secondsRemaining);
            }
            else if (statusResp.status == "idle") {
                QtConcurrent::run([this]() {
                    BasicResponse joinResp = m_api.JoinLobby(m_currentUsername.toStdString());
                    
                    if (joinResp.status != "success") {
                        QMetaObject::invokeMethod(this, [=]() {
                            emit networkError(QString::fromStdString(joinResp.message));
                        }, Qt::QueuedConnection);
                    }
                });
            }
        }, Qt::QueuedConnection);
    });
}

void PreGameNetworkManager::fetchProfile(const QString& username)
{
    QtConcurrent::run([this, username]() {
        ProfileResponse profileResp = m_api.GetProfile(username.toStdString());

        QMetaObject::invokeMethod(this, [=]() {
            if (profileResp.status == "success") {
                emit profileLoaded(
                    QString::fromStdString(profileResp.username),
                    profileResp.score,
                    profileResp.hoursPlayed,
                    profileResp.gamesWon,
                    profileResp.gamesPlayed
                );
            }
            else {
                emit profileError(QString::fromStdString(profileResp.message));
            }
        }, Qt::QueuedConnection);
    });
}

