#include "PreGameNetworkManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QNetworkRequest>

PreGameNetworkManager::PreGameNetworkManager(QObject* parent)
    : QObject(parent)
{
    m_lobbyTimer.setInterval(1000); // 1s
    connect(&m_lobbyTimer, &QTimer::timeout,
        this, &PreGameNetworkManager::onLobbyPollTimeout);
}

// =================== AUTH ==========================

void PreGameNetworkManager::login(const QString& username,
    const QString& password)
{
    QUrl url(m_baseUrl + "/login");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    QJsonDocument doc(obj);

    QNetworkReply* reply = m_manager.post(req, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        int status = reply->attribute(
            QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError) {
            emit loginFailed("Network error: " + reply->errorString());
            return;
        }

        if (status == 200) {
            emit loginSuccess(username);
        }
        else {
            emit loginFailed("Invalid username or password.");
        }
        });
}

void PreGameNetworkManager::registerUser(const QString& username,
    const QString& password)
{
    QUrl url(m_baseUrl + "/register");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject obj;
    obj["username"] = username;
    obj["password"] = password;
    QJsonDocument doc(obj);

    QNetworkReply* reply = m_manager.post(req, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        int status = reply->attribute(
            QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() != QNetworkReply::NoError) {
            emit registerFailed("Network error: " + reply->errorString());
            return;
        }

        if (status == 200 || status == 201) {
            emit registerSuccess(username);
        }
        else if (status == 409) {
            emit registerFailed("Username already exists.");
        }
        else {
            emit registerFailed("Registration failed.");
        }
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
