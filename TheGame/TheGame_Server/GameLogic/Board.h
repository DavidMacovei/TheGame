#pragma once

#include <array>
#include "DrawingDeck.h"
#include "PlacingStack.h"
#include "GameModels.h"

namespace game
{
	class Board
	{
	public:
		Board();

        Card DrawCard();
        bool IsDeckEmpty() const;
		int GetCardsLeftInDeck() const;
        void Reset();
        
        bool CanPlaceCard(uint8_t stackIndex, const Card& card) const;
        bool CanPlaceCardAnywhere(const Card& card) const;
        void PlaceCard(uint8_t stackIndex, Card&& card);

		const std::array<PlacingStack, NUMBER_OF_STACKS>& GetPlacingStacks() const;

	private:
		DrawingDeck m_drawingDeck;
		std::array<PlacingStack, NUMBER_OF_STACKS> m_placingStacks;
	};
}