#pragma once
#include <QWidget>
#include <vector>
#include "CardWidget.h"

class CardHandWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardHandWidget(QWidget* parent = nullptr);

    void setCards(const std::vector<uint8_t>& cards);
    int cardCount() const;

signals:
    void cardClicked(int cardIndex);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<CardWidget*> m_cardWidgets;
    std::vector<uint8_t> m_cards;
    int m_selectedIndex = -1;
    void updateHand();
};