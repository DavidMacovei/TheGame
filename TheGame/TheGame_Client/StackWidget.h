#pragma once
#include <QWidget>
#include <vector>
#include "CardWidget.h"

class StackWidget : public QWidget
{
    Q_OBJECT
public:
    enum class StackType { Ascending, Descending };
    explicit StackWidget(QWidget* parent = nullptr, StackType type = StackType::Ascending);

    void setCards(const std::vector<uint8_t>& cards);
    StackType stackType() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    StackType m_type;
    std::vector<CardWidget*> m_cardWidgets;
    std::vector<uint8_t> m_cards;
    void updateStack();
};