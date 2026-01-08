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

// =================== AUTH ==========================

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

// =================== LOBBY ==========================

void PreGameNetworkManager::joinLobby(const QString& username)
{
    QUrl url(m_baseUrl + "/joinLobby");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject obj;
    obj["username"] = username;
    QJsonDocument doc(obj);

    QNetworkReply* reply = m_manager.post(req, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit networkError("Network error: " + reply->errorString());
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject o = doc.object();

        // Răspunsul actual de la /joinLobby:
        // - status: "Game started" sau "Waiting"
        // - current_players, needed_players, waiting_list[]   (daca Waiting)
        // - players[] (daca Game started)

        const QString status = o.value("status").toString();

        if (status == "Game started") {
            QStringList players;
            auto playersArr = o.value("players");
            if (playersArr.isArray()) {
                for (auto v : playersArr.toArray())
                    players << v.toString();
            }
            emit lobbyGameStarted(players);
        }
        else { // "Waiting"
            int current = o.value("current_players").toInt();
            int needed = o.value("needed_players").toInt();
            QStringList waiting;
            auto wl = o.value("waiting_list");
            if (wl.isArray()) {
                for (auto v : wl.toArray())
                    waiting << v.toString();
            }
            emit lobbyWaiting(waiting, current, needed);
        }
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

void PreGameNetworkManager::onLobbyPollTimeout()
{
    QUrl url(m_baseUrl + "/lobbyState");
    QNetworkRequest req(url);

    QNetworkReply* reply = m_manager.get(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit networkError("Network error: " + reply->errorString());
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject o = doc.object();

        // /lobbyStatus are:
        // - status: "Game running" sau "Waiting"
        // - players[] (daca Game running)
        // - current_players, waiting_list[] (daca Waiting)

        const QString status = o.value("status").toString();

        if (status == "Game running") {
            QStringList players;
            auto arr = o.value("players");
            if (arr.isArray()) {
                for (auto v : arr.toArray())
                    players << v.toString();
            }
            emit lobbyGameStarted(players);
        }
        else {
            int current = o.value("current_players").toInt();
            int needed = 4;
            QStringList waiting;
            auto wl = o.value("waiting_list");
            if (wl.isArray()) {
                for (auto v : wl.toArray())
                    waiting << v.toString();
            }
            emit lobbyWaiting(waiting, current, needed);
        }
        });
}

// =================== PROFILE (stub) ==================

void PreGameNetworkManager::fetchProfile(const QString& username)
{
    //Valori de test, ruta de /profile nu exista
    emit profileLoaded(username, /*score*/ 3, /*hours*/ 1.5);
}
