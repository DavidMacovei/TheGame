#pragma once
#include <QWidget>

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(uint8_t value, QWidget* parent = nullptr);

    void setValue(uint8_t value);
    uint8_t getValue() const;
    void setFlipped(bool flipped); // New method to flip card 180°
    void setRotationAngle(int degrees); // New method to rotate card by any angle

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    uint8_t m_value;
    bool m_flipped = false; // Whether card should be drawn upside down
    int m_rotationAngle = 0; // Rotation angle in degrees
};