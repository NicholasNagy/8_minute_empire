#include "PlayerStrategies.h"



	int GreedyCPU::pickCard(Game* game)
	{
		Player* toPlay = ActionState::toPlay;
		int cardPos = 1;
		int selectedCardPos = 1;
		vector<int> cardsOfIntrest;
		for (Card* card : game->hand()->cardsInHand())
		{
			if (card->getAction()->getID() == 2)  //Build City Action
			{
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney() && toPlay->getNumCities() > 0)
					cardsOfIntrest.push_back(cardPos);
			}
				
			else if (card->getAction()->getID() == 3)  //Destroy Army Action
			{
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney())
					cardsOfIntrest.push_back(cardPos);
			}

			cardPos++;
		}

		//Just pick the cheapest option for now.
		if (cardsOfIntrest.size() > 0)
			selectedCardPos = cardsOfIntrest.at(0);

		return selectedCardPos;
	}




	int  ModerateCPU::pickCard(Game* game)
	{
		return 1;
	}




	int Human::pickCard(Game* game)
	{
		return 0;
	}
