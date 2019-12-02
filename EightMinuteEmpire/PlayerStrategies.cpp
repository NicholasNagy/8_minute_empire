#include "PlayerStrategies.h"
#include <sstream>

using GraphWorld::Map;

std::unordered_map<GraphWorld::Country*, vector<Player*>> opposingArmiesToDestroy;
Player* selectedPlayerToDestroyArmies = nullptr;
GraphWorld::Country* src = nullptr;
GraphWorld::Country* dest = nullptr;

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

		GameplayState::Instance()->updateStatus(ActionState::toPlay->getName() + " has selected handslot " + to_string(position));
		GameplayState::Instance()->updateStatus(to_string(game->hand()->getCardCostAtPosition(position)) + " coins have been deducted from " + ActionState::toPlay->getName());
		GameplayState::Instance()->updateStatus("Card " + ActionState::toPlay->getHand()->getAction()->actionString(false));
	
	}


	void  ModerateCPU::pickCard(Game* game, int position)
	{
		Player* toPlay = ActionState::toPlay;
		int cardPos = 1;
		int selectedCardPos = 1;
		vector<int> playableCards;

		for (Card* card : game->hand()->cardsInHand())
		{

			switch (card->getAction()->getID())
			{
			case 0:  //Place New Armies
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney() && toPlay->getNumArmies() >= game->hand()->getCardAtPosition(cardPos, game->deck(), false)->getAction()->getMultiplicity())
					playableCards.push_back(cardPos);
				break;
			case 1: //Move Armies
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney() )
					playableCards.push_back(cardPos);			
				break;
			case 2: // Build City
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney() && toPlay->getNumCities() > 0)
					playableCards.push_back(cardPos);
				break;
			case 3: //Destroy Army
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney())
					playableCards.push_back(cardPos);
				break;
			case 4:
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney())
					playableCards.push_back(cardPos);
				break;
			case 5:
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney())
					playableCards.push_back(cardPos);
				break;
			case 6: // Ignore
				if (game->hand()->getCardCostAtPosition(cardPos) <= toPlay->getMoney())
					playableCards.push_back(cardPos);
				break;
			default:
				break;
			}

			cardPos++;
		}

		//Just pick the cheapest option for now.
		if (playableCards.size() > 0)
			selectedCardPos = playableCards.at(0);

		cout << " --- " << toPlay->getName() << " Has Selected Handslot " << selectedCardPos << " --- \n";
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(selectedCardPos, game->deck()));
		ActionState::toPlay->getHand()->printCard();
	
	}

	void Human::pickCard(Game* game, int position)
	{

		//Take the passed position (that was clicked)	
		ActionState::toPlay->setCoinPurse(ActionState::toPlay->getMoney() - game->hand()->getCardCostAtPosition(position));
		ActionState::toPlay->setHand(game->hand()->getCardAtPosition(position, game->deck()));
		ActionState::toPlay->getHand()->printCard();

		//Update Observer Status
		GameplayState::Instance()->updateStatus(ActionState::toPlay->getName() + " has selected handslot " + to_string(position));
		GameplayState::Instance()->updateStatus(to_string(game->hand()->getCardCostAtPosition(position)) + " coins have been deducted from " + ActionState::toPlay->getName());
		GameplayState::Instance()->updateStatus("Card " + ActionState::toPlay->getHand()->getAction()->actionString(false));
	}

	void GreedyCPU::playCard(Game* game)
	{
		switch (ActionState::toPlay->getHand()->getAction()->getID())
		{
		case 0:
			ActionState::toPlay->PlaceNewArmies(ActionState::toPlay->getHand()->getAction()->getMultiplicity(), determineCountryForArmyPlacement(game));
			break;
		case 1:
			ActionState::toPlay->MoveArmies(nullptr, nullptr);
			break;
		case 2:
			ActionState::toPlay->BuildCity(determineCountryForCityBuild(game));
			break;
		case 3:
			ActionState::toPlay->DestroyArmy(selectedPlayerToDestroyArmies, determineArmiesToDestroy(game));
			break;
		case 4:
			ActionState::toPlay->Ignore();
			break;
		case 5: 
			ActionState::toPlay->Ignore();
			break;
		case 6:
			ActionState::toPlay->Ignore();
			break;
		default:
			break;
		}

		GameplayState::Instance()->nextMove(game);
	}

	GraphWorld::Country* GreedyCPU::determineCountryForArmyPlacement(Game* game)
	{
	//If no cities have been built, place on starting country.

		GraphWorld::Country* toPlaceOn = Map::instance()->getStartingCountry();
		if (!ActionState::toPlay->countriesWithCities().empty())
		{
			for (GraphWorld::Country* c : ActionState::toPlay->countriesWithCities())
			{
				if (c->getCountryOwner() != ActionState::toPlay)
				{
					toPlaceOn = c;
					break;
				}							
			}
		}
	
	return toPlaceOn;
	}

	GraphWorld::Country* GreedyCPU::determineCountryForCityBuild(Game* game)
	{
		GraphWorld::Country* toBuildOn = Map::instance()->getStartingCountry();
		std::string continent = Map::instance()->getStartingCountry()->getContinent();
		for (int i = 0; i < Map::instance()->getNumCountries(); ++i)
		{
			GraphWorld::Country* c = Map::instance()->getCountry(i);
			if (continent.compare(c->getContinent()) == 0)
			{
				if (!c->getCountryOwner())
				{
					toBuildOn = c;
					break;
				}			
				else if (c->getCountryOwner() != ActionState::toPlay)
				{
					if (c->getCountryOwner()->getHoldings(c)->numArmies() == 1)
					{
						toBuildOn = c;
						break;
					}
				}
			}
		}
		return toBuildOn;
	}

	GraphWorld::Country* GreedyCPU::determineArmiesToDestroy(Game* game)
	{

			bool hasArmiesOnCountry = false;
			GraphWorld::Country* toDestroy = nullptr;
			vector<Player*> otherPlayers;
			for (int i = 0; i < Map::instance()->getNumCountries(); ++i)
			{
				GraphWorld::Country* c = Map::instance()->getCountry(i);
				if (c->getCountryOwner() == ActionState::toPlay)
					continue;

				auto it = c->occupyingPlayers().begin();
				for (int i = 0; i < c->occupyingPlayers().size(); ++i)
				{
				
					if (ActionState::toPlay != it->second)
					{
						if (it->second->getHoldings(c)->numArmies() > 0)
						{
							hasArmiesOnCountry = true;
							otherPlayers.push_back(it->second);
						}
					}
					++it;
				}
				if (hasArmiesOnCountry)
				{
					opposingArmiesToDestroy.try_emplace(c, otherPlayers);
					otherPlayers.clear();
				}
				hasArmiesOnCountry = false;
			}

			for (unordered_map<GraphWorld::Country*, vector<Player*> >::const_iterator it = opposingArmiesToDestroy.begin(); it != opposingArmiesToDestroy.end(); ++it)
			{
				selectedPlayerToDestroyArmies = it->second[0];
				toDestroy = it->first;
			}


			return toDestroy;
	}


	void GreedyCPU::PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country)
	{

		Holdings* playerHoldings = ActionState::toPlay->holdings().at(country->getID());

		//Check if the player has a city on the country or it is a starting country

		playerHoldings->addArmies(numberOfArmies);
		cout << "\nAdded " << numberOfArmies << " Army Unit(s) to Country: " << country->getID() << " For player: " << ActionState::toPlay->getName() << endl;
		country->updateOccupyingPlayerScore(playerHoldings->numArmies() + playerHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setArmies(ActionState::toPlay->getArmies() - numberOfArmies);
	}

	void GreedyCPU::MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
	}

	void GreedyCPU::BuildCity(GraphWorld::Country* country)
	{
		Holdings* countryHoldings = ActionState::toPlay->getHoldings(country);
		countryHoldings->addCities(1);
		cout << "\nBuilt City on Country: " << country->getID() << endl;
		country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setCities(ActionState::toPlay->getNumCities() - 1);
		ActionState::toPlay->countriesWithCities().push_back(country);
	}

	void GreedyCPU::DestroyArmy(Player* player, GraphWorld::Country* country)
	{
		Holdings* countryHoldings = player->getHoldings(country);
		int armies = countryHoldings->numArmies();

		if (armies != 0)
		{
			player->getHoldings(country)->removeArmies(1);
			cout << "\nDestroyed " << player->getName() << "'s Army on Country: " << country->getID() << endl;
			country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), player);
		}
		else
			cout << player->getName() << " Has no armies to Destroy on this Country!\n" << endl;
	}

	void GreedyCPU::Ignore()
	{
		cout << "\nCard Ignored." << endl;
	}

	void ModerateCPU::playCard(Game* game)
	{
		switch (ActionState::toPlay->getHand()->getAction()->getID())
		{
		case 0:
			ActionState::toPlay->PlaceNewArmies(ActionState::toPlay->getHand()->getAction()->getMultiplicity(), determineCountryForArmyPlacement(game));
			break;
		case 1:
			ActionState::toPlay->MoveArmies(nullptr, nullptr);
			break;
		case 2:
			ActionState::toPlay->BuildCity(determineCountryForCityBuild(game));
			break;
		case 3:
			ActionState::toPlay->DestroyArmy(selectedPlayerToDestroyArmies, determineArmiesToDestroy(game));
			break;
		case 4:
			ActionState::toPlay->Ignore();
			break;
		case 5:
			ActionState::toPlay->Ignore();
			break;
		case 6:
			ActionState::toPlay->Ignore();
			break;
		default:
			break;
		}
		GameplayState::Instance()->nextMove(game);
	}

	void ModerateCPU::PlaceNewArmies(int numberOfArmies, GraphWorld::Country* country)
	{
		Holdings* playerHoldings = ActionState::toPlay->holdings().at(country->getID());

		playerHoldings->addArmies(numberOfArmies);
		cout << "\nAdded " << numberOfArmies << " Army Unit(s) to Country: " << country->getID() << " For player: " << ActionState::toPlay->getName() << endl;
		country->updateOccupyingPlayerScore(playerHoldings->numArmies() + playerHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setArmies(ActionState::toPlay->getArmies() - numberOfArmies);
	}

	void ModerateCPU::MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
	}

	void ModerateCPU::BuildCity(GraphWorld::Country* country)
	{

		Holdings* countryHoldings = ActionState::toPlay->getHoldings(country);
		countryHoldings->addCities(1);
		cout << "\nBuilt City on Country: " << country->getID() << endl;
		country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setCities(ActionState::toPlay->getNumCities() - 1);
		ActionState::toPlay->countriesWithCities().push_back(country);
	}

	void ModerateCPU::DestroyArmy(Player* player, GraphWorld::Country* country)
	{
		Holdings* countryHoldings = player->getHoldings(country);
		int armies = countryHoldings->numArmies();

		if (armies != 0)
		{
			player->getHoldings(country)->removeArmies(1);
			cout << "\nDestroyed " << player->getName() << "'s Army on Country: " << country->getID() << endl;
			country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), player);
		}
		else
			cout << player->getName() << " Has no armies to Destroy on this Country!\n" << endl;
	}

	void ModerateCPU::Ignore()
	{
		cout << "\nCard Ignored." << endl;
	}

	GraphWorld::Country* ModerateCPU::determineCountryForArmyPlacement(Game* game)
	{
		GraphWorld::Country* toPlaceOn = Map::instance()->getStartingCountry();
		if (!ActionState::toPlay->countriesWithCities().empty())
		{
			for (GraphWorld::Country* c : ActionState::toPlay->countriesWithCities())
			{
				if (c->getCountryOwner() != ActionState::toPlay)
				{
					toPlaceOn = c;
					break;
				}
			}
		}

		return toPlaceOn;
	}

	GraphWorld::Country* ModerateCPU::determineCountryForCityBuild(Game* game)
	{
		GraphWorld::Country* toBuildOn = Map::instance()->getStartingCountry();
		std::string continent = Map::instance()->getStartingCountry()->getContinent();
		for (int i = 0; i < Map::instance()->getNumCountries(); ++i)
		{
			GraphWorld::Country* c = Map::instance()->getCountry(i);
			if (continent.compare(c->getContinent()) == 0)
			{
				if (!c->getCountryOwner())
				{
					toBuildOn = c;
					break;
				}
				else if (c->getCountryOwner() != ActionState::toPlay)
				{
					if (c->getCountryOwner()->getHoldings(c)->numArmies() == 1)
					{
						toBuildOn = c;
						break;
					}
				}
			}
		}
		return toBuildOn;
	}

	GraphWorld::Country* ModerateCPU::determineArmiesToDestroy(Game* game)
	{
		bool hasArmiesOnCountry = false;
		GraphWorld::Country* toDestroy = nullptr;
		vector<Player*> otherPlayers;
		for (int i = 0; i < Map::instance()->getNumCountries(); ++i)
		{
			GraphWorld::Country* c = Map::instance()->getCountry(i);

			auto it = c->occupyingPlayers().begin();
			for (int i = 0; i < c->occupyingPlayers().size(); ++i)
			{

				if (ActionState::toPlay != it->second)
				{
					if (it->second->getHoldings(c)->numArmies() > 0)
					{
						hasArmiesOnCountry = true;
						otherPlayers.push_back(it->second);
					}
				}
				++it;
			}
			if (hasArmiesOnCountry)
			{
				opposingArmiesToDestroy.try_emplace(c, otherPlayers);
				otherPlayers.clear();
			}
			hasArmiesOnCountry = false;
		}


		for (unordered_map<GraphWorld::Country*, vector<Player*> >::const_iterator it = opposingArmiesToDestroy.begin(); it != opposingArmiesToDestroy.end(); ++it)
		{
			selectedPlayerToDestroyArmies = it->second[0];
			toDestroy = it->first;
		}

		return toDestroy;
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
		cout << "\nAdded " << numberOfArmies << " Army Unit(s) to Country: " << country->getID() << " For player: " << ActionState::toPlay->getName() << endl;
		country->updateOccupyingPlayerScore(playerHoldings->numArmies() + playerHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setArmies(ActionState::toPlay->getArmies()- numberOfArmies);

	}

	void Human::MoveArmies(GraphWorld::Country* srcCountry, GraphWorld::Country* destCountry)
	{
		Holdings* holdingsOnStartCountry = ActionState::toPlay->holdings().at(srcCountry->getID());

		//Check if the player has armies on the starting country
		if (holdingsOnStartCountry->numArmies() == 0)
		{
			cout << "\nNo armies to move." << endl;
			return;
		}

		//Check if the start and destination countries are adjacent to each other
		if (!Map::instance()->getAdjacentList(srcCountry)->isAdjacent(destCountry))
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
			cout << "\nNo more cities left to place.\n";
			return;
		}

		Holdings* countryHoldings = ActionState::toPlay->getHoldings(country);
		countryHoldings->addCities(1);
		cout << "\nBuilt " << 1 << " City on Country: " << country->getID() << endl;
		country->updateOccupyingPlayerScore(countryHoldings->numArmies() + countryHoldings->numCities(), ActionState::toPlay);
		ActionState::toPlay->setCities(ActionState::toPlay->getNumCities() - 1);
		ActionState::toPlay->countriesWithCities().push_back(country);

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
			cout << playerToDestroy->getName() << " Has no armies to Destroy on this Country!\n" << endl;

	}

	void Human::Ignore()
	{
		cout << "\nCard ignored." << endl;

	}
