#include "DeckWidget.h"

DeckWidget::DeckWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(80, 120); // Or adjust as needed
}

void DeckWidget::setCards(const std::vector<uint8_t>& cards)
{
    m_cards = cards;
    updateStack();
}

int DeckWidget::cardCount() const
{
    return static_cast<int>(m_cards.size());
}

void DeckWidget::updateStack()
{
    // Remove old CardWidgets
    for (auto* w : m_cardWidgets) {
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_cardWidgets.clear();

    // Add new CardWidgets, stacked visually
    int overlap = 15; // pixels to overlap
    int y = 0;
    for (size_t i = 0; i < m_cards.size(); ++i) {
        CardWidget* card = new CardWidget(m_cards[i], this);
        card->move(5, y);
        card->show();
        m_cardWidgets.push_back(card);
        y += overlap;
        if (y + card->height() > height()) break; // Don't overflow
    }
}

void DeckWidget::resizeEvent(QResizeEvent*)
{
    updateStack();
}