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

    if (m_value == 0) {
        painter.setBrush(QColor("#1a237e"));
        painter.setPen(QPen(QColor("white"), 2));
        painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 5, 5);

        painter.setPen(QColor("#5c6bc0"));
        painter.setFont(QFont("Arial", 24));
        painter.drawText(rect(), Qt::AlignCenter, "🂠");
    }
    else {
        painter.setBrush(Qt::white);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawRoundedRect(rect().adjusted(2, 2, -2, -2), 5, 5);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 18, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, QString::number(m_value));
    }
}