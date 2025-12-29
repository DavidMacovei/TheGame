#pragma once

#include "DrawingDeck.h"
#include "PlacingStack.h"

#include <array>

namespace game
{
	class Board
	{
	public:
		static const uint8_t NumberOfStacks = 4;

	public:
		Board();

        Card DrawCard();
        bool IsDeckEmpty() const;
		int GetCardsLeftInDeck() const;
        void Reset();
        
        bool CanPlaceCard(uint8_t stackIndex, const Card& card) const;
        bool CanPlaceCardAnywhere(const Card& card) const;
        void PlaceCard(uint8_t stackIndex, Card&& card);

		const std::array<PlacingStack, NumberOfStacks>& GetPlacingStacks() const;

	private:
		DrawingDeck m_drawingDeck;
		std::array<PlacingStack, NumberOfStacks> m_placingStacks;
	};
}