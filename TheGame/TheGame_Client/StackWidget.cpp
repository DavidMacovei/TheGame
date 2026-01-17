#include "StackWidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

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

void StackWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int arrowSize = 18;

    QPolygon arrow;
    if (m_type == StackType::Ascending) {
        // Bottom right, green arrow, facing up
        int marginBR = 14;      // margin from right and bottom
        int yOffsetBR = 57;     // extra offset from bottom if needed
        QPoint bottomRight(width() - marginBR, height() - marginBR - yOffsetBR);

        arrow << QPoint(bottomRight.x() - arrowSize / 2, bottomRight.y())                // left base
              << QPoint(bottomRight.x() + arrowSize / 2, bottomRight.y())                // right base
              << QPoint(bottomRight.x(), bottomRight.y() - arrowSize);                   // tip (upwards)
        painter.setBrush(QBrush(Qt::green));
        painter.setPen(Qt::green);
    } else {
        // Top right, red arrow
        int marginTR = 14;     // margin from right
        int yOffsetTR = -12;     // offset from top
        QPoint topRight(width() - marginTR, marginTR + yOffsetTR);

        arrow << QPoint(topRight.x() - arrowSize / 2, topRight.y())
              << QPoint(topRight.x() + arrowSize / 2, topRight.y())
              << QPoint(topRight.x(), topRight.y() + arrowSize);
        painter.setBrush(QBrush(Qt::red));
        painter.setPen(Qt::red);
    }
    painter.drawPolygon(arrow);
}