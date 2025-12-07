#include "GameBoardWidget.h"
#include "ui_GameBoardWidget.h"
#include <QMessageBox>
#include <QDebug>

GameBoardWidget::GameBoardWidget(const QString& username, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::GameBoardWidget)
    , m_net(new GameNetworkManager(this))
    , m_username(username)
{
    ui->setupUi(this);

    connect(m_net, &GameNetworkManager::gameStateReceived, this, &GameBoardWidget::onGameStateReceived);
    connect(m_net, &GameNetworkManager::moveResult, this, [](bool success, QString msg) {
        if (!success) {
            QMessageBox::warning(nullptr, "Invalid Move", msg);
        }
        });

    connect(ui->btnStack1, &QPushButton::clicked, this, &GameBoardWidget::onStack1Clicked);
    connect(ui->btnStack2, &QPushButton::clicked, this, &GameBoardWidget::onStack2Clicked);
    connect(ui->btnStack3, &QPushButton::clicked, this, &GameBoardWidget::onStack3Clicked);
    connect(ui->btnStack4, &QPushButton::clicked, this, &GameBoardWidget::onStack4Clicked);

    connect(ui->listHand, &QListWidget::itemClicked, this, &GameBoardWidget::onHandItemClicked);
    connect(ui->btnSendChat, &QPushButton::clicked, this, &GameBoardWidget::onSendChatClicked);
    connect(ui->btnEndTurn, &QPushButton::clicked, this, &GameBoardWidget::onEndTurnClicked);
}

GameBoardWidget::~GameBoardWidget()
{
    delete ui;
}

void GameBoardWidget::start()
{
    m_net->startGamePolling();
    ui->lblStatus->setText("Connecting to game...");
}

void GameBoardWidget::onGameStateReceived(const QJsonObject& state)
{
    if (state.contains("stacks")) updateStacks(state["stacks"].toArray());
    if (state.contains("players")) updateHand(state["players"].toArray());
    if (state.contains("chatMessages")) updateChat(state["chatMessages"].toArray());

    updateTurnInfo(state);
}

void GameBoardWidget::updateStacks(const QJsonArray& stacks)
{
    if (stacks.size() >= 4) {
        auto getTop = [](QJsonValue v, bool asc) -> QString {
            QJsonArray arr = v.toArray();
            if (arr.isEmpty()) return asc ? "1" : "100";
            return QString::number(arr.last().toInt());
            };

        ui->btnStack1->setText("ASC (1 -> 99)\nTOP: " + getTop(stacks[0], true));
        ui->btnStack2->setText("ASC (1 -> 99)\nTOP: " + getTop(stacks[1], true));
        ui->btnStack3->setText("DESC (100 -> 2)\nTOP: " + getTop(stacks[2], false));
        ui->btnStack4->setText("DESC (100 -> 2)\nTOP: " + getTop(stacks[3], false));
    }
}