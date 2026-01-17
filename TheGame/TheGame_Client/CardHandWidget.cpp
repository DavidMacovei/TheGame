#include "CardHandWidget.h"
#include <cmath>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

CardHandWidget::CardHandWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(90);
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

void CardHandWidget::setOrientation(Orientation orientation)
{
    m_orientation = orientation;
    updateHand();
}

void CardHandWidget::setRotation(int degrees)
{
    m_rotationDegrees = degrees;
    updateHand(); // Redraw with rotation
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

    int cardWidth = 50, cardHeight = 70, overlap = 35;

    if (m_orientation == Orientation::Horizontal) {
        int totalWidth = cardWidth + (n - 1) * overlap;
        int startX = std::max(0, (width() - totalWidth) / 2);
        int y = 10;

        bool flipped = (m_rotationDegrees == 180);
        
        for (int i = 0; i < n; ++i) {
            CardWidget* card = new CardWidget(m_cards[i], this);
            if (i == m_selectedIndex) {
                 int offsetY = flipped ? 20 : -20;
                card->move(startX + i * overlap, y + offsetY);
     card->setStyleSheet("border: 3px solid yellow; background: white;");
           card->raise();
            } else {
     card->move(startX + i * overlap, y);
              card->setStyleSheet("");
            }

    if (m_rotationDegrees == 180) {
              card->setRotationAngle(180);
            }

            card->show();
            m_cardWidgets.push_back(card);
        }
    } else {

        bool isRotatedSideways = (m_rotationDegrees == 90 || m_rotationDegrees == 270);

        int effectiveCardHeight = isRotatedSideways ? cardWidth : cardHeight;
        int effectiveCardWidth = isRotatedSideways ? cardHeight : cardWidth;
        
        int totalHeight = effectiveCardHeight + (n - 1) * overlap;
    int startY = std::max(0, (height() - totalHeight) / 2);

        int x = std::max(15, (width() - effectiveCardWidth) / 2);

   for (int i = 0; i < n; ++i) {
  CardWidget* card = new CardWidget(m_cards[i], this);

   if (isRotatedSideways) {
 card->setFixedSize(80, 60);
   }

   if (i == m_selectedIndex) {
  card->move(x - 15, startY + i * overlap);
                card->setStyleSheet("border: 3px solid yellow; background: white;");
        card->raise();
 } else {
      card->move(x, startY + i * overlap);
     card->setStyleSheet("");
    }

   if (m_rotationDegrees == 90) {
  card->setRotationAngle(90);
  } else if (m_rotationDegrees == 270) {
        card->setRotationAngle(270);
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

            updateHand();
            break;
        }
    }

    QWidget::mousePressEvent(event);
}

void CardHandWidget::resizeEvent(QResizeEvent*)
{
    updateHand();
}