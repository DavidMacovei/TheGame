#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class GameOverWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameOverWidget(QWidget* parent = nullptr);
    
    void showVictory(int totalCards = 98);
    void showDefeat(int cardsLeft, const QString& reason = "");

signals:
    void backToLobby();

private:
    void setupUI();
    void applyVictoryStyle();
    void applyDefeatStyle();

    QLabel* m_titleLabel;
    QLabel* m_messageLabel;
    QLabel* m_statsLabel;
    QPushButton* m_backButton;
};
