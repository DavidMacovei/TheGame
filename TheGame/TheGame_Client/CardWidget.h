#pragma once
#include <QWidget>

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(uint8_t value, QWidget* parent = nullptr);

    void setValue(uint8_t value);
    uint8_t getValue() const;
    void setFlipped(bool flipped); 
    void setRotationAngle(int degrees); 

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    uint8_t m_value;
    bool m_flipped = false; 
    int m_rotationAngle = 0; 
};