#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QLabel>
#include "DeckWidget.h"
#include "GameNetworkManager.h"

namespace Ui { class GameBoardWidget; }

class GameBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoardWidget(const QString& username, QWidget* parent = nullptr);
    ~GameBoardWidget();

    void start();

signals:
    void gameEnded(bool victory, int cardsLeft);

private slots:
    void onGameStateReceived(const QJsonObject& state);

    void onHandItemClicked(QListWidgetItem* item);
    void onStack1Clicked();
    void onStack2Clicked();
    void onStack3Clicked();
    void onStack4Clicked();
    void onSendChatClicked();
    void onEndTurnClicked();

private:
    Ui::GameBoardWidget* ui;
    GameNetworkManager* m_net;
    QString m_username;

    int m_selectedCardIndex = -1;

    void updateStacks(const QJsonArray& stacks);
    void updateHand(const QJsonArray& players);
    void updateOpponentHands(const QJsonArray& players);
    void updateChat(const QJsonArray& messages);
    void updateTurnInfo(const QJsonObject& state);

    void handleStackClick(int stackId);
    void clearLayout(QLayout* layout);
    QWidget* createCardBack();
};