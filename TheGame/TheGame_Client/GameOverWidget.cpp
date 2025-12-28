#include "GameOverWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

GameOverWidget::GameOverWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void GameOverWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(30);
    
    m_titleLabel = new QLabel(this);
    QFont titleFont;
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    m_messageLabel = new QLabel(this);
    QFont messageFont;
    messageFont.setPointSize(20);
    m_messageLabel->setFont(messageFont);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    
    m_statsLabel = new QLabel(this);
    QFont statsFont;
    statsFont.setPointSize(14);
    m_statsLabel->setFont(statsFont);
    m_statsLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_backButton = new QPushButton("Back to Lobby", this);
    m_backButton->setMinimumSize(150, 50);
    m_backButton->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border-radius: 5px; font-size: 16px; }"
        "QPushButton:hover { background-color: #2980b9; }"
    );
    connect(m_backButton, &QPushButton::clicked, this, &GameOverWidget::backToLobby);
    
    m_statsButton = new QPushButton("View Stats", this);
    m_statsButton->setMinimumSize(150, 50);
    m_statsButton->setStyleSheet(
        "QPushButton { background-color: #95a5a6; color: white; border-radius: 5px; font-size: 16px; }"
        "QPushButton:hover { background-color: #7f8c8d; }"
    );
    connect(m_statsButton, &QPushButton::clicked, this, &GameOverWidget::viewStats);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_backButton);
    buttonLayout->addWidget(m_statsButton);
    buttonLayout->addStretch();
    
    mainLayout->addStretch();
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_messageLabel);
    mainLayout->addWidget(m_statsLabel);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void GameOverWidget::showVictory(int totalCards)
{
    m_titleLabel->setText("🎉 VICTORY! 🎉");
    m_messageLabel->setText("Congratulations! You've mastered The Game!");
    m_statsLabel->setText(QString("All %1 cards successfully placed!").arg(totalCards));
    
    applyVictoryStyle();
}

void GameOverWidget::showDefeat(int cardsLeft, const QString& reason)
{
    m_titleLabel->setText("💀 DEFEAT 💀");
    
    QString message = reason.isEmpty() 
        ? "The team couldn't complete the game..." 
        : reason;
    m_messageLabel->setText(message);
    
    m_statsLabel->setText(QString("%1 cards remaining in hands and deck").arg(cardsLeft));
    
    applyDefeatStyle();
}

void GameOverWidget::applyVictoryStyle()
{
    setStyleSheet(
        "GameOverWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #27ae60, stop:1 #2ecc71); }"
    );
    m_titleLabel->setStyleSheet("color: white;");
    m_messageLabel->setStyleSheet("color: white;");
    m_statsLabel->setStyleSheet("color: #ecf0f1;");
}

void GameOverWidget::applyDefeatStyle()
{
    setStyleSheet(
        "GameOverWidget { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #c0392b, stop:1 #e74c3c); }"
    );
    m_titleLabel->setStyleSheet("color: white;");
    m_messageLabel->setStyleSheet("color: white;");
    m_statsLabel->setStyleSheet("color: #ecf0f1;");
}
