#include "CardHandWidget.h"
#include <cmath>
#include <QMouseEvent>

CardHandWidget::CardHandWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(90); // Adjust as needed for card size
}

void CardHandWidget::setCards(const std::vector<uint8_t>& cards)
{
    m_cards = cards;
    updateHand();
}

int CardHandWidget::cardCount() const
{
    return static_cast<int>(m_cards.size());
}

void CardHandWidget::updateHand()
{
    for (auto* w : m_cardWidgets) {
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_cardWidgets.clear();

    int n = static_cast<int>(m_cards.size());
    if (n == 0) return;

    int cardWidth = 50, cardHeight = 70, overlap = 30;
    int totalWidth = cardWidth + (n - 1) * overlap;
    int startX = std::max(0, (width() - totalWidth) / 2);
    int y = 10;

    for (int i = 0; i < n; ++i) {
        CardWidget* card = new CardWidget(m_cards[i], this);
        card->move(startX + i * overlap, y);
        card->show();
        m_cardWidgets.push_back(card);
    }
}

void CardHandWidget::mousePressEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();

    for (int i = m_cardWidgets.size() - 1; i >= 0; --i) {
        CardWidget* card = m_cardWidgets[i];
        QRect cardRect = card->geometry();

        if (cardRect.contains(clickPos)) {
            m_selectedIndex = i;
            emit cardClicked(i);

            update();
            break;
        }
    }

    QWidget::mousePressEvent(event);
}

void CardHandWidget::resizeEvent(QResizeEvent*)
{
    updateHand();
}