#include "StackWidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

StackWidget::StackWidget(QWidget* parent, StackType type)
    : QWidget(parent), m_type(type)
{
    setFixedSize(60, 120);
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
    for (auto* w : m_cardWidgets) {
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_cardWidgets.clear();

    int overlap = 18;
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
        int marginBR = 14;      
        int yOffsetBR = 57;     
        QPoint bottomRight(width() - marginBR, height() - marginBR - yOffsetBR);

        arrow << QPoint(bottomRight.x() - arrowSize / 2, bottomRight.y())
              << QPoint(bottomRight.x() + arrowSize / 2, bottomRight.y()) 
              << QPoint(bottomRight.x(), bottomRight.y() - arrowSize);
        painter.setBrush(QBrush(Qt::green));
        painter.setPen(Qt::green);
    } else {
        int marginTR = 14;     
        int yOffsetTR = -12;    
        QPoint topRight(width() - marginTR, marginTR + yOffsetTR);

        arrow << QPoint(topRight.x() - arrowSize / 2, topRight.y())
              << QPoint(topRight.x() + arrowSize / 2, topRight.y())
              << QPoint(topRight.x(), topRight.y() + arrowSize);
        painter.setBrush(QBrush(Qt::red));
        painter.setPen(Qt::red);
    }
    painter.drawPolygon(arrow);
}