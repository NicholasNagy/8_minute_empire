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
		//Take the passed position (that was clicked)
		cout << " --- " << toPlay->getName() << " Has Selected Handslot " << position << " --- \n";
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(position, game->deck()));
		ActionState::toPlay->getHand()->printCard();
	}

	void GreedyCPU::playCard(Game* game)
	{
		switch (ActionState::toPlay->getHand()->getAction()->getID())
		{
		case 0:
			ActionState::toPlay->PlaceNewArmies(1, nullptr);
			break;
		case 1:
			ActionState::toPlay->MoveArmies(game->getMap(), nullptr, nullptr);
			break;
		case 2:
			ActionState::toPlay->BuildCity(nullptr);
			break;
		case 3:
			ActionState::toPlay->DestroyArmy(nullptr, nullptr);
			break;
		case 4:
		
			break;
		case 5: 
			
			break;
		case 6:
			
			break;
		default:
			break;
		}
	}

	void GreedyCPU::PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country)
	{
	}

	void GreedyCPU::MoveArmies(GraphWorld::Map*, GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
	}

	void GreedyCPU::BuildCity(GraphWorld::Country* country)
	{
	}

	void GreedyCPU::DestroyArmy(Player* player, GraphWorld::Country* country)
	{
	}

	void GreedyCPU::Ignore()
	{
	}

	void ModerateCPU::playCard(Game* game)
	{
		switch (ActionState::toPlay->getHand()->getAction()->getID())
		{
		case 0:
			ActionState::toPlay->PlaceNewArmies(1, nullptr);
			break;
		case 1:
			ActionState::toPlay->MoveArmies(game->getMap(), nullptr, nullptr);
			break;
		case 2:
			ActionState::toPlay->BuildCity(nullptr);
			break;
		case 3:
			ActionState::toPlay->DestroyArmy(nullptr, nullptr);
			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
		default:
			break;
		}
	}

	void ModerateCPU::PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country)
	{
	}

	void ModerateCPU::MoveArmies(GraphWorld::Map*, GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
	}

	void ModerateCPU::BuildCity(GraphWorld::Country* country)
	{
	}

	void ModerateCPU::DestroyArmy(Player* player, GraphWorld::Country* country)
	{
	}

	void ModerateCPU::Ignore()
	{
	}

	void Human::playCard(Game* game)
	{
		ActionState::inActionState = true;
		switch (ActionState::toPlay->getHand()->getAction()->getID())
		{
		case 0:
			
			game->pushState(PlaceNewArmiesState::Instance());
			break;
		case 1:
			game->pushState(MoveArmiesState::Instance());
			break;
		case 2:
			game->pushState(BuildCityState::Instance());
			break;
		case 3:
			game->pushState(DestroyArmyState::Instance());
			break;
		case 4: //TODO: and
			game->pushState(IgnoreState::Instance());
			break;
		case 5:  //TODO: or
			game->pushState(IgnoreState::Instance());
			break;
		case 6:  
			game->pushState(IgnoreState::Instance());
			break;
		default:
			break;
		}
	}

	void Human::PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country)
	{
		string messageFail = "Cannot place a new army on country " + to_string(country->getID());

		if (ActionState::toPlay->getArmies() == 0)
		{
			cout << "No more armies left to place.\n";
			return;
		}

		Holdings* playerHoldings = ActionState::toPlay->holdings().at(country->getID());

		//Check if the player has a city on the country or it is a starting country

		if (playerHoldings->numCities() == 0 && !country->isStartCountry())
		{
			cout << messageFail << endl;
			return;
		}
		playerHoldings->addArmies(numberOfArmies);
		cout << "Added " << numberOfArmies << " Army Unit(s) to Country: " << country->getID() << " For player: " << ActionState::toPlay->getName() << endl;
		country->updateOccupyingPlayerScore(playerHoldings->numArmies() + playerHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setArmies(ActionState::toPlay->getArmies()- numberOfArmies);

	}

	void Human::MoveArmies(GraphWorld::Map* map, GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
		Holdings* holdingsOnStartCountry = ActionState::toPlay->holdings().at(srcCountry->getID());

		//Check if the player has armies on the starting country
		if (holdingsOnStartCountry->numArmies() == 0)
		{
			cout << "\nNo armies to move." << endl;
			return;
		}

		//Check if the start and destination countries are adjacent to each other
		if (!map->getAdjacentList(srcCountry)->isAdjacent(destCountry))
		{
			cout << "Cannot move armies there." << endl;
			return;
		}

		//Check if the move requires naval
		if (srcCountry->isNavalCountry() && destCountry->isNavalCountry())
		{
			cout << "Cannot move armies there. Requires Naval movement." << endl;
			return;
		}

		//Move armies
		Holdings* holdingsOnDestCountry = ActionState::toPlay->holdings().at(destCountry->getID());
		holdingsOnStartCountry->removeArmies(1);
		holdingsOnDestCountry->addArmies(1);
		cout << "\nMoved " << 1 << " Army from Country: " << srcCountry->getID() << " to Country: " << destCountry->getID() << endl;
		srcCountry->updateOccupyingPlayerScore(holdingsOnStartCountry->numArmies() + holdingsOnStartCountry->numCities(), ActionState::toPlay);
		destCountry->updateOccupyingPlayerScore(holdingsOnDestCountry->numArmies() + holdingsOnDestCountry->numCities(), ActionState::toPlay);
	}

	void Human::BuildCity(GraphWorld::Country* country)
	{
		if (ActionState::toPlay->getNumCities() == 0)
		{
			cout << "No more cities left to place.\n";
			return;
		}

		Holdings* countryHoldings = ActionState::toPlay->getHoldings(country);
		countryHoldings->addCities(1);
		cout << "\nBuilt " << 1 << " City on Country: " << country->getID() << endl;
		country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setCities(ActionState::toPlay->getNumCities() - 1);

	}

	void Human::DestroyArmy(Player* playerToDestroy, GraphWorld::Country* country)
	{

		Holdings* countryHoldings = playerToDestroy->getHoldings(country);
		int armies = countryHoldings->numArmies();

		if (armies != 0)
		{
			playerToDestroy->getHoldings(country)->removeArmies(1);
			cout << "\nDestroyed " << playerToDestroy->getName() << "'s Army on Country: " << country->getID() << endl;
			country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), playerToDestroy);
		}
		else
			cout << playerToDestroy << " Has no armies to Destroy on this Country!\n" << endl;

	}

	void Human::Ignore()
	{
		cout << "Card ignored." << endl;

	}
