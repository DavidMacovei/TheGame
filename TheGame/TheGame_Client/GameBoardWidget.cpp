#include "GameBoardWidget.h"
#include "ui_GameBoardWidget.h"
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>

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
    if (state.contains("players")) {
        updateHand(state["players"].toArray());
        updateOpponentHands(state["players"].toArray());
    }
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

        ui->btnStack1->setText("ASC ?\n1 ? 99\nTOP: " + getTop(stacks[0], true));
        ui->btnStack2->setText("ASC ?\n1 ? 99\nTOP: " + getTop(stacks[1], true));
        ui->btnStack3->setText("DESC ?\n100 ? 2\nTOP: " + getTop(stacks[2], false));
        ui->btnStack4->setText("DESC ?\n100 ? 2\nTOP: " + getTop(stacks[3], false));
    }
}

void GameBoardWidget::updateHand(const QJsonArray& players)
{
    QJsonArray myHand;
    for (const auto& p : players) {
        QJsonObject pObj = p.toObject();
        if (pObj["username"].toString() == m_username) {
            myHand = pObj["hand"].toArray();
            break;
        }
    }

    if (ui->listHand->count() != myHand.size()) {
        ui->listHand->clear();
        for (const auto& c : myHand) {
            ui->listHand->addItem(QString::number(c.toInt()));
        }
        m_selectedCardIndex = -1;
    }
}

void GameBoardWidget::updateChat(const QJsonArray& messages)
{
    if (ui->listChat->count() != messages.size()) {
        ui->listChat->clear();
        for (const auto& m : messages) {
            QJsonObject mObj = m.toObject();
            QString line = mObj["username"].toString() + ": " + mObj["text"].toString();
            ui->listChat->addItem(line);
        }
        ui->listChat->scrollToBottom();
    }
}

void GameBoardWidget::updateTurnInfo(const QJsonObject& state)
{
    int currentIdx = state["currentPlayer"].toInt();
    QJsonArray players = state["players"].toArray();

    if (currentIdx >= 0 && currentIdx < players.size()) {
        QString currentPlayer = players[currentIdx].toObject()["username"].toString();

        if (currentPlayer == m_username) {
            ui->lblStatus->setText("?? ESTE RANDUL TAU!");
            ui->lblStatus->setStyleSheet("font-size: 16pt; font-weight: bold; color: #ffd700; background-color: transparent;");

            ui->btnStack1->setEnabled(true);
            ui->btnStack2->setEnabled(true);
            ui->btnStack3->setEnabled(true);
            ui->btnStack4->setEnabled(true);
            ui->btnEndTurn->setEnabled(true);
        }
        else {
            ui->lblStatus->setText("? Asteptam jucatorul: " + currentPlayer);
            ui->lblStatus->setStyleSheet("font-size: 14pt; color: white; background-color: transparent;");

            ui->btnStack1->setEnabled(false);
            ui->btnStack2->setEnabled(false);
            ui->btnStack3->setEnabled(false);
            ui->btnStack4->setEnabled(false);
            ui->btnEndTurn->setEnabled(false);
        }
    }
}

void GameBoardWidget::onHandItemClicked(QListWidgetItem* item)
{
    m_selectedCardIndex = ui->listHand->row(item);
}

void GameBoardWidget::handleStackClick(int stackId)
{
    if (m_selectedCardIndex == -1) {
        QMessageBox::information(this, "Info", "Selecteaza o carte din mana mai intai!");
        return;
    }

    m_net->sendMove(m_username, m_selectedCardIndex, stackId);
}

void GameBoardWidget::onStack1Clicked() { handleStackClick(0); }
void GameBoardWidget::onStack2Clicked() { handleStackClick(1); }
void GameBoardWidget::onStack3Clicked() { handleStackClick(2); }
void GameBoardWidget::onStack4Clicked() { handleStackClick(3); }

void GameBoardWidget::onSendChatClicked()
{
    QString msg = ui->lineChatMsg->text().trimmed();
    if (!msg.isEmpty()) {
        m_net->sendChatMessage(m_username, msg);
        ui->lineChatMsg->clear();
    }
}

void GameBoardWidget::onEndTurnClicked()
{
    m_net->endTurn(m_username);
}

void GameBoardWidget::clearLayout(QLayout* layout)
{
    if (!layout) return;
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

QWidget* GameBoardWidget::createCardBack()
{
    QWidget* card = new QWidget();
    card->setFixedSize(50, 70);
    card->setStyleSheet(
        "background-color: #1a237e;"
        "border: 2px solid #fff;"
        "border-radius: 5px;"
    );

    QLabel* pattern = new QLabel("??", card);
    pattern->setAlignment(Qt::AlignCenter);
    pattern->setStyleSheet("color: #5c6bc0; font-size: 24px; border: none; background: transparent;");
    pattern->setGeometry(0, 0, 50, 70);

    return card;
}