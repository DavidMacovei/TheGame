#pragma once
#include <QWidget>

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(uint8_t value, QWidget* parent = nullptr);

    void setValue(uint8_t value);
    uint8_t getValue() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    uint8_t m_value;
};