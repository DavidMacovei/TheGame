#pragma once
#include <QWidget>
#include <vector>
#include "CardWidget.h"

class CardHandWidget : public QWidget
{
    Q_OBJECT
public:
    enum class Orientation {
        Horizontal,  // For bottom/top players
      Vertical     // For left/right players
    };

    explicit CardHandWidget(QWidget* parent = nullptr);

    void setCards(const std::vector<uint8_t>& cards);
    int cardCount() const;
    void setOrientation(Orientation orientation);
    void setRotation(int degrees); // Support 0, 90, 180, 270 degrees
    
    // Inline definition to avoid linker issues
    void clearSelection() {
        m_selectedIndex = -1;
     updateHand();
    }

signals:
    void cardClicked(int cardIndex);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<CardWidget*> m_cardWidgets;
  std::vector<uint8_t> m_cards;
    int m_selectedIndex = -1;
    Orientation m_orientation = Orientation::Horizontal;
    int m_rotationDegrees = 0; // Store rotation (0 or 180)
    void updateHand();
};