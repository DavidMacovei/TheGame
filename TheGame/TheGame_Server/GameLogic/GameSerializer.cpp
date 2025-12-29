#include "GameSerializer.h"
#include "GameModels.h"

namespace game
{
	std::string SerializeGameState(const Game& gameObj, uint8_t requestingPlayerIndex)
	{

		GameState gameState;

		gameState.status = ToString(gameObj.GetStatus());
		gameState.currentPlayer = gameObj.GetCurrentPlayerIndex();
		gameState.drawDeckCount = gameObj.GetBoard().GetCardsLeftInDeck();
		gameState.minCardsToPlay = gameObj.GetMinimumNumberOfCardsToPlay();

		const std::array<PlacingStack, numberOfStacks>& placingStacks = gameObj.GetBoard().GetPlacingStacks();
		for (size_t i = 0; i < numberOfStacks; i++)
		{
			StackState stackState;

			stackState.topCardValue = placingStacks[i].GetCurrentValue();
			if (placingStacks[i].GetType() == StackType::Ascending)
				stackState.isAscending = true;
			else
				stackState.isAscending = false;

			gameState.placingStacks[i] = stackState;
		}

		const std::vector<Player>& players = gameObj.GetPlayers();
		for (size_t i = 0; i < players.size(); i++)
		{
			const auto& serverPlayer = players[i];

			PlayerState playerState;
			playerState.cardCount = serverPlayer.GetCardsInHand();
			playerState.username = serverPlayer.GetUsername();

			if (i == requestingPlayerIndex)
			{
				const auto& hand = serverPlayer.GetHand();
				for (const auto& card : hand)
				{
					playerState.hand.push_back(card.GetValue());
				}
			}
			
			gameState.players.push_back(playerState);
		}

		json j = gameState;
		return j.dump();
	}
}
