#include "GameNetworkManager.h"
#include <QDebug>

GameNetworkManager::GameNetworkManager(const QString& username, ClientApi* api, QObject* parent)
    : QObject(parent)
    , m_api(api)
    , m_username(username)
{
    m_gameTimer.setInterval(1500);
    connect(&m_gameTimer, &QTimer::timeout, this, &GameNetworkManager::onPollTimeout);
}

void GameNetworkManager::startGamePolling()
{
    if (!m_gameTimer.isActive()) {
      m_gameTimer.start();
    }
}

void GameNetworkManager::stopGamePolling()
{
    m_gameTimer.stop();
}

void GameNetworkManager::setPlayerIndex(int playerIndex)
{
    m_playerIndex = playerIndex;
}

void GameNetworkManager::onPollTimeout()
{
    try {
        GameState state = m_api->GetGameState(m_username.toStdString());
        QJsonObject jsonState = convertGameStateToJson(state);
     emit gameStateReceived(jsonState);
    }
    catch (const std::exception& e) {
        qWarning() << "Failed to get game state:" << e.what();
        emit networkError(QString("Failed to get game state: %1").arg(e.what()));
    }
}

void GameNetworkManager::sendMove(int cardIndex, int stackId)
{
    if (m_playerIndex < 0) {
        emit moveResult(false, "Player index not set");
        return;
    }

    try {
        BasicResponse response = m_api->PlayCard(m_playerIndex, cardIndex, stackId);
        bool success = (response.status == "success");
        emit moveResult(success, QString::fromStdString(response.message));
    }
    catch (const std::exception& e) {
        qWarning() << "Failed to send move:" << e.what();
        emit moveResult(false, QString("Failed to send move: %1").arg(e.what()));
    }
}

void GameNetworkManager::sendChatMessage(const QString& message)
{
 try {
        BasicResponse response = m_api->sendMessage(m_username.toStdString(), message.toStdString());
     if (response.status != "success") {
            qWarning() << "Failed to send chat message:" << QString::fromStdString(response.message);
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Failed to send chat message:" << e.what();
    }
}

void GameNetworkManager::endTurn()
{
    if (m_playerIndex < 0) {
        emit networkError("Player index not set");
        return;
    }

    try {
        BasicResponse response = m_api->EndTurn(m_playerIndex);
        if (response.status != "success") {
       emit networkError(QString::fromStdString(response.message));
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Failed to end turn:" << e.what();
        emit networkError(QString("Failed to end turn: %1").arg(e.what()));
    }
}

QJsonObject GameNetworkManager::convertGameStateToJson(const GameState& state)
{
    QJsonObject obj;
    
    // Status
    obj["status"] = QString::fromStdString(state.status);
    obj["currentPlayer"] = state.currentPlayer;
    obj["drawDeckCount"] = state.drawDeckCount;
    obj["minCardsToPlay"] = state.minCardsToPlay;
    
    // Placing stacks
    QJsonArray stacksArray;
    for (const auto& stack : state.placingStacks) {
        QJsonObject stackObj;
 stackObj["topCardValue"] = stack.topCardValue;
        stackObj["isAscending"] = stack.isAscending;
     stacksArray.append(stackObj);
    }
    obj["placingStacks"] = stacksArray;
    
    // Players
    QJsonArray playersArray;
    for (const auto& player : state.players) {
        QJsonObject playerObj;
  playerObj["username"] = QString::fromStdString(player.username);
        playerObj["cardCount"] = player.cardCount;
        
        // Add hand array if available
        QJsonArray handArray;
        for (int card : player.hand) {
     handArray.append(card);
  }
    playerObj["hand"] = handArray;
        
   playersArray.append(playerObj);
    }
    obj["players"] = playersArray;
    
    // Fetch chat messages separately
    try {
        ChatHistory chatHistory = m_api->GetChatHistory();
        QJsonArray chatArray;
        for (const auto& msg : chatHistory.messages) {
   QJsonObject msgObj;
            msgObj["username"] = QString::fromStdString(msg.sender);
   msgObj["text"] = QString::fromStdString(msg.message);
 chatArray.append(msgObj);
        }
        obj["chatMessages"] = chatArray;
    }
    catch (const std::exception& e) {
      qWarning() << "Failed to get chat history:" << e.what();
    obj["chatMessages"] = QJsonArray(); // Empty array on error
}
    
    return obj;
}