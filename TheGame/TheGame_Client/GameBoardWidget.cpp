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

    // Replace placeholder widgets with properly typed StackWidgets
    // Find and remove placeholders
    QWidget* placeholder1 = findChild<QWidget*>("stackPlaceholder1");
    QWidget* placeholder2 = findChild<QWidget*>("stackPlaceholder2");
    QWidget* placeholder3 = findChild<QWidget*>("stackPlaceholder3");
    QWidget* placeholder4 = findChild<QWidget*>("stackPlaceholder4");

    if (placeholder1) {
        ui->stacksLayout->removeWidget(placeholder1);
        placeholder1->deleteLater();
    }
    if (placeholder2) {
 ui->stacksLayout->removeWidget(placeholder2);
    placeholder2->deleteLater();
    }
    if (placeholder3) {
        ui->stacksLayout->removeWidget(placeholder3);
        placeholder3->deleteLater();
    }
    if (placeholder4) {
        ui->stacksLayout->removeWidget(placeholder4);
 placeholder4->deleteLater();
    }

    // Create properly typed StackWidgets
    m_stack1 = new StackWidget(this, StackWidget::StackType::Ascending);
    m_stack2 = new StackWidget(this, StackWidget::StackType::Ascending);
    m_stack3 = new StackWidget(this, StackWidget::StackType::Descending);
    m_stack4 = new StackWidget(this, StackWidget::StackType::Descending);

    m_stack1->setObjectName("stack1");
    m_stack2->setObjectName("stack2");
    m_stack3->setObjectName("stack3");
    m_stack4->setObjectName("stack4");

    m_stack1->setMinimumSize(QSize(80, 140));
    m_stack2->setMinimumSize(QSize(80, 140));
    m_stack3->setMinimumSize(QSize(80, 140));
  m_stack4->setMinimumSize(QSize(80, 140));

    ui->stacksLayout->addWidget(m_stack1, 0, 0);
    ui->stacksLayout->addWidget(m_stack2, 0, 1);
    ui->stacksLayout->addWidget(m_stack3, 1, 0);
    ui->stacksLayout->addWidget(m_stack4, 1, 1);

 // Make stack widgets clickable
    m_stack1->installEventFilter(this);
    m_stack2->installEventFilter(this);
    m_stack3->installEventFilter(this);
    m_stack4->installEventFilter(this);

    connect(m_net, &GameNetworkManager::gameStateReceived, this, &GameBoardWidget::onGameStateReceived);
    connect(m_net, &GameNetworkManager::moveResult, this, [](bool success, QString msg) {
        if (!success) {
  QMessageBox::warning(nullptr, "Invalid Move", msg);
        }
    });

    connect(ui->btnSendChat, &QPushButton::clicked, this, &GameBoardWidget::onSendChatClicked);
    connect(ui->btnEndTurn, &QPushButton::clicked, this, &GameBoardWidget::onEndTurnClicked);
    
    setupCardHandConnections();
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

void GameBoardWidget::setupCardHandConnections()
{
    connect(ui->mainPlayerHand, &CardHandWidget::cardClicked, this, &GameBoardWidget::onCardSelected);
}

bool GameBoardWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == m_stack1) {
            onStack1Clicked();
    return true;
      } else if (obj == m_stack2) {
   onStack2Clicked();
          return true;
        } else if (obj == m_stack3) {
         onStack3Clicked();
  return true;
        } else if (obj == m_stack4) {
            onStack4Clicked();
      return true;
   }
    }
    return QWidget::eventFilter(obj, event);
}

void GameBoardWidget::onGameStateReceived(const QJsonObject& state)
{
    // Check if game has ended (Won or Lost)
    if (state.contains("status")) {
        QString status = state["status"].toString();
 
if (status == "Won") {
     m_net->stopGamePolling();
            emit gameEnded(true, 0);
      return;
        }
      else if (status == "Lost") {
            int cardsLeft = 0;
    
    if (state.contains("drawDeckCount")) {
         cardsLeft += state["drawDeckCount"].toInt();
      }
        
            if (state.contains("players")) {
    QJsonArray players = state["players"].toArray();
 for (const auto& p : players) {
       QJsonObject pObj = p.toObject();
                  if (pObj.contains("hand")) {
  cardsLeft += pObj["hand"].toArray().size();
      } else if (pObj.contains("cardCount")) {
       cardsLeft += pObj["cardCount"].toInt();
      }
         }
     }
   
    m_net->stopGamePolling();
            emit gameEnded(false, cardsLeft);
            return;
   }
    }
    
    if (state.contains("placingStacks")) updateStacks(state["placingStacks"].toArray());
    if (state.contains("drawDeckCount")) updateDeck(state["drawDeckCount"].toInt());
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
        if (stacks.size() > 0) {
            QJsonObject stack1Obj = stacks[0].toObject();
      int topValue = stack1Obj["topCardValue"].toInt();
     std::vector<uint8_t> cards = {static_cast<uint8_t>(topValue)};
       m_stack1->setCards(cards);
      }

  if (stacks.size() > 1) {
         QJsonObject stack2Obj = stacks[1].toObject();
     int topValue = stack2Obj["topCardValue"].toInt();
  std::vector<uint8_t> cards = {static_cast<uint8_t>(topValue)};
      m_stack2->setCards(cards);
  }

        if (stacks.size() > 2) {
  QJsonObject stack3Obj = stacks[2].toObject();
  int topValue = stack3Obj["topCardValue"].toInt();
 std::vector<uint8_t> cards = {static_cast<uint8_t>(topValue)};
  m_stack3->setCards(cards);
  }

   if (stacks.size() > 3) {
      QJsonObject stack4Obj = stacks[3].toObject();
       int topValue = stack4Obj["topCardValue"].toInt();
  std::vector<uint8_t> cards = {static_cast<uint8_t>(topValue)};
      m_stack4->setCards(cards);
 }
    }
}

void GameBoardWidget::updateDeck(int deckCount)
{
    ui->lblDeckCount->setText(QString("Deck: %1").arg(deckCount));
  
    std::vector<uint8_t> deckCards;
    int cardsToShow = qMin(deckCount, 10);
    for (int i = 0; i < cardsToShow; ++i) {
        deckCards.push_back(0); // 0 represents card back
 }
    ui->drawDeck->setCards(deckCards);
}

void GameBoardWidget::updateHand(const QJsonArray& players)
{
    for (const auto& p : players) {
        QJsonObject pObj = p.toObject();
        if (pObj["username"].toString() == m_username) {
        QJsonArray myHandArray = pObj["hand"].toArray();
       std::vector<uint8_t> myHand;
         for (const auto& c : myHandArray) {
 myHand.push_back(static_cast<uint8_t>(c.toInt()));
   }
            ui->mainPlayerHand->setCards(myHand);
  break;
    }
    }
}

void GameBoardWidget::updateOpponentHands(const QJsonArray& players)
{
    // Find opponents (players other than current user)
    QVector<QPair<QString, QJsonArray>> opponents;
    for (const auto& p : players) {
     QJsonObject pObj = p.toObject();
  QString name = pObj["username"].toString();
     if (name != m_username) {
    QJsonArray hand = pObj["hand"].toArray();
     opponents.append({name, hand});
 }
    }

    // Update player labels and hands based on position
    // Player 1 = Left, Player 2 = Top-Left, Player 3 = Top-Right, Player 4 = Right
    QList<CardHandWidget*> handWidgets = {
        ui->player1Hand,
 ui->player2Hand,
        ui->player3Hand,
        ui->player4Hand
    };
    QList<QLabel*> nameLabels = {
     ui->lblPlayer1Name,
        ui->lblPlayer2Name,
        ui->lblPlayer3Name,
        ui->lblPlayer4Name
    };

    for (int i = 0; i < 4; i++) {
     if (i < opponents.size()) {
  int cardCount = opponents[i].second.size();
    nameLabels[i]->setText(opponents[i].first + " (" + QString::number(cardCount) + " cards)");
         nameLabels[i]->setVisible(true);
            
            // Create vector of card backs (0 = card back)
            std::vector<uint8_t> cardBacks(cardCount, 0);
  handWidgets[i]->setCards(cardBacks);
        } else {
    nameLabels[i]->setText("Waiting...");
         nameLabels[i]->setStyleSheet("color: gray; font-style: italic; background-color: transparent;");
   handWidgets[i]->setCards(std::vector<uint8_t>());
        }
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
  ui->lblStatus->setText("🎯 ESTE RANDUL TAU!");
         ui->lblStatus->setStyleSheet("font-size: 16pt; font-weight: bold; color: #ffd700; background-color: transparent;");

   m_stack1->setEnabled(true);
            m_stack2->setEnabled(true);
      m_stack3->setEnabled(true);
      m_stack4->setEnabled(true);
     ui->btnEndTurn->setEnabled(true);
  }
     else {
 ui->lblStatus->setText("⏳ Asteptam jucatorul: " + currentPlayer);
  ui->lblStatus->setStyleSheet("font-size: 14pt; color: white; background-color: transparent;");

      m_stack1->setEnabled(false);
            m_stack2->setEnabled(false);
      m_stack3->setEnabled(false);
 m_stack4->setEnabled(false);
   ui->btnEndTurn->setEnabled(false);
 }
    }
}

void GameBoardWidget::onCardSelected(int cardIndex)
{
    m_selectedCardIndex = cardIndex;
}

void GameBoardWidget::handleStackClick(int stackId)
{
    if (m_selectedCardIndex == -1) {
        QMessageBox::information(this, "Info", "Selecteaza o carte din mana mai intai!");
        return;
}

    m_net->sendMove(m_username, m_selectedCardIndex, stackId);
    m_selectedCardIndex = -1; // Reset selection after move attempt
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