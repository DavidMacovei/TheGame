#pragma once
#include"CardWidget.h"
#include <QWidget>
#include <stack>

class DeckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeckWidget(QWidget* parent = nullptr);
    void setCardCount(int count);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    std::stack<CardWidget> m_cards;
    int m_cardCount = 0;
};


