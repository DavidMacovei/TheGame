#pragma once

#include <QWidget>
#include <QStringList>
#include "PreGameNetworkManager.h"

namespace Ui {
    class LobbyWidget;
}

class LobbyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LobbyWidget(PreGameNetworkManager* net, QWidget* parent = nullptr);
    ~LobbyWidget();

    void enterLobby(const QString& username);

signals:
    void backToLogin();
    void gameStarted(QStringList players);
    void viewProfile(QString username);

private slots:
    void onCancelClicked();
    void onProfileClicked();
    void onLobbyWaiting(QStringList players, int current, int needed);
    void onLobbyGameStarted(QStringList players);

private:
    Ui::LobbyWidget* ui;
    PreGameNetworkManager* net;
    QString username;

    int secondsLeft = 30;
    void updateTimerLabel();
};
