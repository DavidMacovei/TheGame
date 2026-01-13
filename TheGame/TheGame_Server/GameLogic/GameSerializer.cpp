#include "GameSerializer.h"
#include "GameModels.h"

namespace game
{
	std::string SerializeGameState(const Game& gameObj, const std::string& requestingUsername)
	{

		GameState gameState;

		gameState.status = ToString(gameObj.GetStatus());
		gameState.currentPlayer = gameObj.GetCurrentPlayerIndex();
		gameState.drawDeckCount = gameObj.GetBoard().GetCardsLeftInDeck();
		gameState.minCardsToPlay = gameObj.GetMinimumNumberOfCardsToPlay();

		const auto& placingStacks = gameObj.GetBoard().GetPlacingStacks();

		std::transform(placingStacks.begin(), placingStacks.end(), gameState.placingStacks.begin(),
			[](const PlacingStack& stack) {
				StackState stackState;
				stackState.topCardValue = stack.GetCurrentValue();
				stackState.isAscending = (stack.GetType() == StackType::Ascending);
				return stackState;
			});

		const auto& players = gameObj.GetPlayers();

		gameState.players.clear();

		std::transform(players.begin(), players.end(), std::back_inserter(gameState.players),
			[&requestingUsername](const Player& serverPlayer) {
				PlayerState playerState;
				playerState.cardCount = serverPlayer.GetCardsInHand();
				playerState.username = serverPlayer.GetUsername();

				if (serverPlayer.GetUsername() == requestingUsername)
				{
					const auto& hand = serverPlayer.GetHand();
					for (const auto& card : hand)
						playerState.hand.push_back(card.GetValue());
				}

				return playerState;
			});

		json j = gameState;
		return j.dump();
	}
}
