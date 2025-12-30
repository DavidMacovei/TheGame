#pragma once
#include <QWidget>
#include <vector>
#include "CardWidget.h"

class DeckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeckWidget(QWidget* parent = nullptr);

    void setCards(const std::vector<uint8_t>& cards);
    int cardCount() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    std::vector<CardWidget*> m_cardWidgets;
    std::vector<uint8_t> m_cards;
    void updateStack();
};