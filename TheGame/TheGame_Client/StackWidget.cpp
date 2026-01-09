#include "StackWidget.h"

StackWidget::StackWidget(QWidget* parent, StackType type)
    : QWidget(parent), m_type(type)
{
    setFixedSize(60, 120); // Adjust as needed
}

void StackWidget::setCards(const std::vector<uint8_t>& cards)
{
    m_cards = cards;
    updateStack();
}

StackWidget::StackType StackWidget::stackType() const
{
    return m_type;
}

void StackWidget::updateStack()
{
    // Remove old CardWidgets
    for (auto* w : m_cardWidgets) {
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_cardWidgets.clear();

    // Add new CardWidgets, stacked visually
    int overlap = 18; // pixels to overlap
    int y = 0;
    int n = static_cast<int>(m_cards.size());
    for (int i = 0; i < n; ++i) {
        int idx = (m_type == StackType::Ascending) ? i : (n - 1 - i);
        CardWidget* card = new CardWidget(m_cards[idx], this);
        card->move(5, y);
        card->show();
        m_cardWidgets.push_back(card);
        y += overlap;
        if (y + card->height() > height()) break;
    }
}

void StackWidget::resizeEvent(QResizeEvent*)
{
    updateStack();
}