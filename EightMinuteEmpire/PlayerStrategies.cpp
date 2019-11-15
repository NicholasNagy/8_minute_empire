#include "PlayerStrategies.h"



	void GreedyCPU::pickCard(Game* game, int position)
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


		cout << " --- " << toPlay->getName() <<  " Has Selected Handslot " << selectedCardPos << " --- \n";
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(selectedCardPos, game->deck()));
		ActionState::toPlay->getHand()->printCard();
	
	}




	void  ModerateCPU::pickCard(Game* game, int position)
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

		cout << " --- " << toPlay->getName() << " Has Selected Handslot " << selectedCardPos << " --- \n";
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(selectedCardPos, game->deck()));
		ActionState::toPlay->getHand()->printCard();
	
	}




	void Human::pickCard(Game* game, int position)
	{
		Player* toPlay = ActionState::toPlay;
		cout << " --- " << toPlay->getName() << " Has Selected Handslot " << position << " --- \n";
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(position, game->deck()));
		ActionState::toPlay->getHand()->printCard();
	}