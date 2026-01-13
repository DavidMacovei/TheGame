#include "LobbyWidget.h"
#include "ui_LobbyWidget.h"
#include <QMessageBox>

LobbyWidget::LobbyWidget(PreGameNetworkManager* net, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LobbyWidget)
    , net(net)
{
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked,
        this, &LobbyWidget::onCancelClicked);

    connect(ui->profileButton, &QPushButton::clicked,
        this, &LobbyWidget::onProfileClicked);

    connect(net, &PreGameNetworkManager::lobbyWaiting,
        this, &LobbyWidget::onLobbyWaiting);

    connect(net, &PreGameNetworkManager::lobbyGameStarted,
        this, &LobbyWidget::onLobbyGameStarted);

    connect(net, &PreGameNetworkManager::networkError, this,
        [&](QString msg) {
            QMessageBox::warning(this, "Network Error", msg);
        });
}

LobbyWidget::~LobbyWidget()
{
    delete ui;
}

void LobbyWidget::enterLobby(const QString& username)
{
    this->username = username;

    ui->statusLabel->setText("Joining lobby...");
    ui->playersList->clear();
    updateTimerLabel(30);

    net->joinLobby(username);

    net->startLobbyPolling();
}

void LobbyWidget::onCancelClicked()
{
    net->leaveLobby(username);
    net->stopLobbyPolling();
    emit backToLogin();
}

void LobbyWidget::onProfileClicked()
{
    emit viewProfile(username);
}

void LobbyWidget::onLobbyWaiting(QStringList players,
    int current,
    int needed,
    int secondsRemaining)
{
    ui->statusLabel->setText(
        QString("Waiting for players: %1 / %2").arg(current).arg(needed)
    );

    ui->playersList->clear();
    ui->playersList->addItems(players);

    updateTimerLabel(secondsRemaining);
}

void LobbyWidget::onLobbyGameStarted(QStringList players)
{
    net->stopLobbyPolling();
    ui->statusLabel->setText("Game started!");

    ui->playersList->clear();
    ui->playersList->addItems(players);

    emit gameStarted(players);
}

void LobbyWidget::updateTimerLabel(int seconds)
{
    ui->timerLabel->setText(
        QString("00:%1").arg(seconds, 2, 10, QChar('0'))
    );
}
