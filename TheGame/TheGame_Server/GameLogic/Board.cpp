#include "Board.h"

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
		if (stackIndex >= NumberOfStacks)
			return false;

		return m_placingStacks[stackIndex].CanPlace(card);
	}

	bool Board::CanPlaceCardAnywhere(const Card& card) const
	{
		for (const auto& stack : m_placingStacks)
			if (stack.CanPlace(card))
				return true;
		return false;
	}

	void Board::PlaceCard(uint8_t stackIndex, Card&& card)
	{
		m_placingStacks[stackIndex].PlaceCard(std::move(card));
	}

	const std::array<PlacingStack, Board::NumberOfStacks>& Board::GetPlacingStacks() const
	{
		return m_placingStacks;
	}
}