#include "Board.h"
#include <algorithm>

namespace game
{
	Board::Board() 
	{
		Reset();
	}

	Card Board::DrawCard()
	{
		return m_drawingDeck.DrawCard();
	}

	bool Board::IsDeckEmpty() const
	{
		return m_drawingDeck.IsEmpty();
	}

	int Board::GetCardsLeftInDeck() const 
	{
		return m_drawingDeck.GetLeftoverCardNumber();
	}

	void Board::Reset()
	{
		m_drawingDeck = DrawingDeck();
		m_drawingDeck.Shuffle();

		m_placingStacks = {
			PlacingStack(StackType::Ascending),
			PlacingStack(StackType::Ascending),
			PlacingStack(StackType::Descending),
			PlacingStack(StackType::Descending)
		};
	}

	bool Board::CanPlaceCard(uint8_t stackIndex, const Card& card) const
	{
		if (stackIndex >= NUMBER_OF_STACKS)
			return false;

		return m_placingStacks[stackIndex].CanPlace(card);
	}

	bool Board::CanPlaceCardAnywhere(const Card& card) const
	{
		return std::ranges::any_of(m_placingStacks, [&](const auto& stack) {
			return stack.CanPlace(card);
			});
	}

	void Board::PlaceCard(uint8_t stackIndex, Card&& card)
	{
		m_placingStacks[stackIndex].PlaceCard(std::move(card));
	}

	const std::array<PlacingStack, NUMBER_OF_STACKS>& Board::GetPlacingStacks() const
	{
		return m_placingStacks;
	}
}