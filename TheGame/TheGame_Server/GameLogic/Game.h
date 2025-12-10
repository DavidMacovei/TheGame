#pragma once

#include "GameStatus.h"
#include "Player.h"
#include "DrawingDeck.h"
#include "PlacingStack.h"
#include "GameModels.h"
#include <vector>
#include <array>

namespace game {
	const uint8_t numberOfStacks = 4;

	class Game
	{
	public:
		Game(const std::vector<std::string>& playerNames);
		bool PlayCard(uint8_t playerIndex, uint8_t handIndex, uint8_t stackIndex);
		bool EndTurn(uint8_t playerIndex);
		GameStatus GetStatus() const;
		int GetCurrentPlayerIndex() const;
		const std::vector<Player>& GetPlayers() const;
		const std::array<PlacingStack, numberOfStacks>& GetPlacingStacks() const;
		int GetMinimumNumberOfCardsToPlay() const;
		void InitializeGame(const std::vector<std::string>& playerNames);
		void NextPlayer();
		void UpdateGameStatus();
		bool CurrentPlayerCanPlay() const;
		std::string GetGameStateAsJson(uint8_t requestingPlayerIndex) const;

	private:
		DrawingDeck m_drawingDeck;
		std::array<PlacingStack, numberOfStacks> m_placingStacks;
		std::vector<Player> m_players;
		GameStatus m_status;
		int m_minimumNumberOfCardsToPlay;
		int m_cardsPlayedThisTurn;
		int m_currentPlayerIndex;
	};
}

