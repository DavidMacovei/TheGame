#include "DeckWidget.h"
#include <QPainter>

DeckWidget::DeckWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(80, 120); // Set your desired size
}

void DeckWidget::setCardCount(int count)
{
    m_cardCount = count;
    update(); // triggers repaint
}

void DeckWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw deck rectangle
    painter.setBrush(QColor("#eeeeee"));
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(2, 2, -2, -2));

    // Draw card count
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(m_cardCount));
}
