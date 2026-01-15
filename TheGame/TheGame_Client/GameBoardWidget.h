#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QEvent>
#include "CardHandWidget.h"
#include "StackWidget.h"
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

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onGameStateReceived(const QJsonObject& state);
    void onStack1Clicked();
    void onStack2Clicked();
    void onStack3Clicked();
    void onStack4Clicked();
    void onSendChatClicked();
    void onEndTurnClicked();
    void onCardSelected(int cardIndex);

private:
    Ui::GameBoardWidget* ui;
    GameNetworkManager* m_net;
    QString m_username;

    // Stack widgets (created manually since they need StackType parameter)
    StackWidget* m_stack1;
    StackWidget* m_stack2;
    StackWidget* m_stack3;
    StackWidget* m_stack4;

    int m_selectedCardIndex = -1;

    void updateStacks(const QJsonArray& stacks);
    void updateHand(const QJsonArray& players);
    void updateOpponentHands(const QJsonArray& players);
    void updateChat(const QJsonArray& messages);
    void updateTurnInfo(const QJsonObject& state);
    void updateDeck(int deckCount);

    void handleStackClick(int stackId);
    void setupCardHandConnections();
};