#pragma once
#include <QWidget>

class DeckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeckWidget(QWidget* parent = nullptr);
    void setCardCount(int count);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    int m_cardCount = 0;
};


