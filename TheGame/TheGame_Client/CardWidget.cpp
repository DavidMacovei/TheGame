#include "CardWidget.h"
#include <QPainter>

CardWidget::CardWidget(uint8_t value, QWidget* parent)
    : QWidget(parent), m_value(value)
{
    setFixedSize(50, 70); // Adjust size as needed
}

void CardWidget::setValue(uint8_t value)
{
    m_value = value;
    update();
}

uint8_t CardWidget::getValue() const
{
    return m_value;
}

void CardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw card rectangle
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(2, 2, -2, -2));

    // Draw value centered
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(m_value));
}