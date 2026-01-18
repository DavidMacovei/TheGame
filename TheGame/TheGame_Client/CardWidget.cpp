#include "CardWidget.h"
#include <QPainter>

CardWidget::CardWidget(uint8_t value, QWidget* parent)
    : QWidget(parent), m_value(value), m_flipped(false), m_rotationAngle(0)
{
    setFixedSize(50, 70);
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

void CardWidget::setFlipped(bool flipped)
{
    m_flipped = flipped;
    update();
}

void CardWidget::setRotationAngle(int degrees)
{
    m_rotationAngle = degrees;
    update();
}

void CardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (m_rotationAngle != 0) {
        painter.translate(width() / 2.0, height() / 2.0);
        painter.rotate(m_rotationAngle);

        if (m_rotationAngle == 90 || m_rotationAngle == 270) {
            painter.translate(-25, -35);
        } else {
            painter.translate(-width() / 2.0, -height() / 2.0);
        }
    }
    else if (m_flipped) {
        painter.translate(width() / 2.0, height() / 2.0);
        painter.rotate(180);
        painter.translate(-width() / 2.0, -height() / 2.0);
    }

    QRect cardRect = (m_rotationAngle == 90 || m_rotationAngle == 270) ?
        QRect(0, 0, 50, 70) : rect();

    if (m_value == 0) {
        
        painter.setBrush(QColor("#1a237e"));
        painter.setPen(QPen(QColor("white"), 2));
        painter.drawRoundedRect(cardRect.adjusted(2, 2, -2, -2), 5, 5);

        painter.setPen(QColor("#5c6bc0"));
        painter.setFont(QFont("Arial", 24));
        painter.drawText(cardRect, Qt::AlignCenter, "🂠");
    }
    else {
        
        painter.setBrush(Qt::white);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawRoundedRect(cardRect.adjusted(2, 2, -2, -2), 5, 5);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 18, QFont::Bold));
        painter.drawText(cardRect, Qt::AlignCenter, QString::number(m_value));
    }
}