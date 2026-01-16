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
    m_selectedIndex = -1; // Reset selection when cards change
    updateHand();
}

int CardHandWidget::cardCount() const
{
    return static_cast<int>(m_cards.size());
}

void CardHandWidget::setOrientation(Orientation orientation)
{
    m_orientation = orientation;
    updateHand();
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

    if (m_orientation == Orientation::Horizontal) {
        // Horizontal layout (bottom/top players)
        int totalWidth = cardWidth + (n - 1) * overlap;
        int startX = std::max(0, (width() - totalWidth) / 2);
        int y = 10;

        for (int i = 0; i < n; ++i) {
            CardWidget* card = new CardWidget(m_cards[i], this);

            // If this card is selected, raise it up
            if (i == m_selectedIndex) {
                card->move(startX + i * overlap, y - 20); // 20px higher
                card->setStyleSheet("border: 3px solid yellow; background: white;");
                card->raise(); // Bring to front (z-index)
            } else {
                card->move(startX + i * overlap, y);
                card->setStyleSheet("");
            }

            card->show();
            m_cardWidgets.push_back(card);
        }
    } else {
        // Vertical layout (left/right players)
        int totalHeight = cardHeight + (n - 1) * overlap;
        int startY = std::max(0, (height() - totalHeight) / 2);
        int x = 10;

        for (int i = 0; i < n; ++i) {
            CardWidget* card = new CardWidget(m_cards[i], this);

            // If this card is selected, push it out
            if (i == m_selectedIndex) {
                card->move(x - 15, startY + i * overlap); // 15px to the left
                card->setStyleSheet("border: 3px solid yellow; background: white;");
                card->raise(); // Bring to front
            } else {
                card->move(x, startY + i * overlap);
                card->setStyleSheet("");
            }

            card->show();
            m_cardWidgets.push_back(card);
        }
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

            updateHand(); // Redraw to show selection
            break;
        }
    }

    QWidget::mousePressEvent(event);
}

void CardHandWidget::resizeEvent(QResizeEvent*)
{
    updateHand();
}