#include "ActionState.h"
#include "Map.h"
#include "Player.h"
#include <sstream>

PlaceNewArmiesState PlaceNewArmiesState::mPlaceNewArmiesState;
MoveArmiesState MoveArmiesState::mMoveArmiesState;
BuildCityState BuildCityState::mBuildCityState;
DestroyArmyState DestroyArmyState::mDestroyArmyState;
IgnoreState IgnoreState::mIgnoreState;
bool ActionState::inActionState = false;

SDL_Rect cursor;

Player* ActionState::toPlay = nullptr;
std::vector<GraphWorld::Country*> selectedCountries; //the countries selected by the user

//Action State base class

void ActionState::init(Game* game)
{
}

void ActionState::resume()
{
	GameplayState::resume();
}

void ActionState::pause()
{
	GameplayState::pause();
}

void ActionState::clean(Game* game)
{
	GameplayState::clean(game);
}

void ActionState::handleEvents(Game* game)
{
	GameplayState::handleEvents(game);
}

void ActionState::draw(Game* game)
{
	GameplayState::draw(game);
}

void ActionState::update(Game* game)
{
	GameplayState::update(game);
}



// Place New Armies Action State

void PlaceNewArmiesState::init(Game* game)
{
	cout << "\nSelected Place New Armies Action\n";
	cout << "\nSelect a country with an owned city to place your armies. \n";
}

void PlaceNewArmiesState::pause()
{
	ActionState::pause();
}

void PlaceNewArmiesState::resume()
{
	ActionState::resume();
}

void PlaceNewArmiesState::clean(Game* game)
{
}

void PlaceNewArmiesState::handleEvents(Game* game)
{
	ActionState::handleEvents(game);
	switch (GameplayState::event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (GameplayState::event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (GameplayState::event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		PlaceNewArmiesState::getSelectedCountry(game);
		break;

	case SDL_KEYDOWN:
	{
		switch (GameplayState::event.key.keysym.sym)
		{
		case SDLK_RETURN:
			if (!selectedCountries.empty())
			{
			executeAction();
			selectedCountries.clear();
			game->popState();
			}
			break;
		default:
			break;
		}
	}

	default:
		break;
		}

}

void PlaceNewArmiesState::draw(Game* game)
{
	ActionState::draw(game);
}

void PlaceNewArmiesState::update(Game* game)
{
	ActionState::update(game);
}

void PlaceNewArmiesState::executeAction()
{
	//Places all the armies on the last selected country
		ActionState::toPlay->PlaceNewArmies(ActionState::toPlay->getHand()->getAction()->getMultiplicity(), selectedCountries.back());

}

void PlaceNewArmiesState::getSelectedCountry(Game* game)
{

	//Find out type of tile clicked
	static int typeCol;
	static int typeRow;
	static int type;

	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = game->getMap()->getTileMap()->tiles[typeRow][typeCol];
		GraphWorld::Country* clickedON = nullptr;
		if (type < game->getMap()->getNumCountries() && type >= 0)
		{
			clickedON = game->getMap()->getCountry(type);
			if (ActionState::toPlay->getHoldings(clickedON)->numCities() == 0 && !clickedON->isStartCountry())
			{
				std::cout << "Cannot place a New Army on this Country. Player does not own a city here." << std::endl;
				std::cout << "Please select another Country " << std::endl;
			}
			else
			{
				selectedCountries.push_back(clickedON);
				std::cout << "\nSelected Country: " << clickedON->getID() << std::endl;
				std::cout << "Press 'ENTER' To confirm move\n" << std::endl;
			}
		}
	}
}

//Move Armies Action State
GraphWorld::Country* srcCountry = nullptr;
GraphWorld::Country* destCountry = nullptr;

void MoveArmiesState::init(Game* game)
{
	cout << "\nSelected Move Armies Action\n\n";
	cout << "Please select a country to move your armies from: \n";
}

void MoveArmiesState::pause()
{
	ActionState::pause();
}

void MoveArmiesState::resume()
{
	ActionState::resume();
}

void MoveArmiesState::clean(Game* game)
{

}

void MoveArmiesState::handleEvents(Game* game)
{
	ActionState::handleEvents(game);
	switch (GameplayState::event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (GameplayState::event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (GameplayState::event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		MoveArmiesState::getSelectedCountry(game);
		break;

	case SDL_KEYDOWN:
	{
		switch (GameplayState::event.key.keysym.sym)
		{
		case SDLK_RETURN:
			if (srcCountry && destCountry)
			{
				executeAction(game);
				game->popState();
				srcCountry = destCountry = nullptr;

			}

			break;
		default:
			break;
		}
	}

	default:
		break;
	}
}

void MoveArmiesState::draw(Game* game)
{
	ActionState::draw(game);
}

void MoveArmiesState::update(Game* game)
{
	ActionState::update(game);
}

void MoveArmiesState::getSelectedCountry(Game* game)
{
	//Find out type of tile clicked
	static int typeCol;
	static int typeRow;
	static int type;

	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = game->getMap()->getTileMap()->tiles[typeRow][typeCol];

		if (type < game->getMap()->getNumCountries() && type >= 0)
		{
			if (srcCountry)
			{
				destCountry = game->getMap()->getCountry(type);
				std::cout << "\nSELECTED ARMY MOVE FROM {COUNTRY " << srcCountry->getID()
						  << "} TO {COUNTRY " << destCountry->getID() << "}" << std::endl;
				std::cout << "Press 'ENTER' To confirm move" << std::endl;

			}
			else
				srcCountry = game->getMap()->getCountry(type);

			//Check if the player has any armies on this country 
			if (ActionState::toPlay->getHoldings(srcCountry)->numArmies() > 0)
			{
				if (!destCountry)
				{
					std::cout << "\nWILL MOVE ARMIES FROM COUNTRY " << srcCountry->getID() << std::endl;
					std::cout << "Select a country to move to: " << std::endl;
				}		
			}
			else
			{
				std::cout << "No Armies to Move on Country " << srcCountry->getID() << std::endl;
				std::cout << "\nPlease select another Country " << std::endl;
				srcCountry = nullptr;
			}
		}
	}
}

void MoveArmiesState::executeAction(Game* game)
{
	toPlay->MoveArmies(game->getMap(), srcCountry, destCountry);
}

//Build City Action State

void BuildCityState::init(Game* game)
{
	cout << "\nSelected Build City Action\n";
	cout << "\nSelect a country to build your city. \n";
}

void BuildCityState::pause()
{
	ActionState::pause();
}

void BuildCityState::resume()
{
	ActionState::resume();
}

void BuildCityState::clean(Game* game)
{
}

void BuildCityState::handleEvents(Game* game)
{
	ActionState::handleEvents(game);
	switch (GameplayState::event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (GameplayState::event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (GameplayState::event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		BuildCityState::getSelectedCountry(game);
		break;

	case SDL_KEYDOWN:
	{
		switch (GameplayState::event.key.keysym.sym)
		{
		case SDLK_RETURN:
			if (!selectedCountries.empty())
			{
				executeAction(game);
				selectedCountries.clear();
				game->popState();
			}
			break;

		default:
			break;
		}
	}
	default:
		break;
	}
}

void BuildCityState::draw(Game* game)
{
	ActionState::draw(game);
}

void BuildCityState::update(Game* game)
{
	ActionState::update(game);
}

void BuildCityState::getSelectedCountry(Game* game)
{
	//Find out type of tile clicked
	static int typeCol;
	static int typeRow;
	static int type;

	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = game->getMap()->getTileMap()->tiles[typeRow][typeCol];
		GraphWorld::Country* clickedON = nullptr;
		if (type < game->getMap()->getNumCountries() && type >= 0)
		{
			clickedON = game->getMap()->getCountry(type);
			selectedCountries.push_back(clickedON);
			std::cout << "\nSelected Country: " << clickedON->getID() << std::endl;
			std::cout << "Press 'ENTER' To confirm move" << std::endl;

		}
	}
}

void BuildCityState::executeAction(Game* game)
{
	ActionState::toPlay->BuildCity(selectedCountries.back());
}


//Destroy Army Action State
std::unordered_map<GraphWorld::Country*, vector<Player*>> opposingArmies; //countries where opposing players have armies.
Player* selectedPlayer = nullptr;
void DestroyArmyState::init(Game* game)
{
	cout << "\nSelected Destroy City Action\n";
	
//Print other player armies on the map.
	std::stringstream ss;
	bool hasArmiesOnCountry = false;
	vector<Player*> otherPlayers;
	std::cout << "\nCountries with enemy armies: " << std::endl;
	for (int i = 0; i < game->getMap()->getNumCountries(); ++i)
	{
			GraphWorld::Country* c = game->getMap()->getCountry(i);
			
			auto it = c->occupyingPlayers().begin();
			for (int i = 0; i < c->occupyingPlayers().size(); ++i)
			{
				//If not the current player, print number of armies on this country
				if (ActionState::toPlay != it->second)
				{
					if (it->second->getHoldings(c)->numArmies() > 0)
					{
						hasArmiesOnCountry = true;
						ss << "  " <<it->second->getName() << ": Number of Armies: " << it->second->getHoldings(c)->numArmies() << std::endl;
						otherPlayers.push_back(it->second);					
					}				
				}
				++it;
			}
			if (hasArmiesOnCountry)
			{
				std::cout << "-COUNTRY " << c->getID() << ": \n";
				std::cout << ss.str();
				ss.str(std::string());
				opposingArmies.try_emplace(c, otherPlayers);
				otherPlayers.clear();
			}
			hasArmiesOnCountry = false;
	}

	cout << "\nSelect a country with enemy armies you wish to destroy. \n";
}

void DestroyArmyState::pause()
{
	ActionState::pause();
}

void DestroyArmyState::resume()
{
	ActionState::resume();
}

void DestroyArmyState::clean(Game* game)
{
}

void DestroyArmyState::handleEvents(Game* game)
{
	ActionState::handleEvents(game);
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		DestroyArmyState::getSelectedCountry(game);
		break;

	case SDL_KEYDOWN:
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_1:
			getSelectedPlayer(game, 0);
			break;
		case SDLK_2:
			getSelectedPlayer(game, 1);
			break;
		case SDLK_3:
			getSelectedPlayer(game, 2);
			break;
		case SDLK_4:
			getSelectedPlayer(game, 3);
			break;

		case SDLK_RETURN:
			if (selectedCountries.back() && selectedPlayer)
			{
				executeAction(game);
				selectedCountries.clear();
				opposingArmies.clear();
				selectedPlayer = nullptr;
				game->popState();
			}
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
}

void DestroyArmyState::draw(Game* game)
{
	ActionState::draw(game);
}

void DestroyArmyState::update(Game* game)
{
	ActionState::update(game);
}

void DestroyArmyState::getSelectedCountry(Game* game)
{
	//Find out type of tile clicked
	static int typeCol;
	static int typeRow;
	static int type;

	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = game->getMap()->getTileMap()->tiles[typeRow][typeCol];
		GraphWorld::Country* clickedON = nullptr;
		if (type < game->getMap()->getNumCountries() && type >= 0)
		{
			clickedON = game->getMap()->getCountry(type);
			//If the clicked country has opposing armies on it
			if (opposingArmies.count(clickedON))
			{
				selectedCountries.push_back(clickedON);
				std::cout << "\nSelected Country: " << clickedON->getID() << std::endl;
				std::cout << "Enemy Armies on this country: " << std::endl;
				int n = 1;
				for (Player* p : opposingArmies.at(clickedON))
				{				
					cout << n << ". " << p->getName() <<": Number of Armies: " << p->getHoldings(clickedON)->numArmies() << std::endl;
					n++;
				}
					std::cout << "\nSelect the number representing the player to destroy their army" << std::endl;
			}
		}
	}
}

void DestroyArmyState::getSelectedPlayer(Game* game, int index)
{
	if (selectedCountries.back())
	{
		if (index >= opposingArmies.at(selectedCountries.back()).size())
		{
			std::cout << "No armies for that player on this country." << std::endl;
		}
		else 
		{
			selectedPlayer = opposingArmies.at(selectedCountries.back()).at(index);
			std::cout << "\nWILL DESTROY ARMIES OF {" << selectedPlayer->getName() << "} ON {COUNTRY: "<< selectedCountries.back()->getID() << "}" << std::endl;
			std::cout << "Press 'ENTER' To confirm move" << std::endl;
		}
	}
}


void DestroyArmyState::executeAction(Game* game)
{
	ActionState::toPlay->DestroyArmy(selectedPlayer, selectedCountries.back());
}

//Ignore Action State

void IgnoreState::init(Game* game)
{
	std::cout << "\nPress 'ENTER' To Ignore card and go to the next turn" << std::endl;
}

void IgnoreState::pause()
{
	ActionState::pause();
}

void IgnoreState::resume()
{
	ActionState::resume();
}

void IgnoreState::clean(Game* game)
{
}

void IgnoreState::handleEvents(Game* game)
{
	ActionState::handleEvents(game);
	switch (GameplayState::event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (GameplayState::event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (GameplayState::event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		break;

	case SDL_KEYDOWN:
	{
		switch (GameplayState::event.key.keysym.sym)
		{
		case SDLK_RETURN:
				executeAction(game);
				game->popState();
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
}

void IgnoreState::draw(Game* game)
{
	ActionState::draw(game);
}

void IgnoreState::update(Game* game)
{
	ActionState::update(game);
}

void IgnoreState::executeAction(Game* game)
{
	ActionState::toPlay->Ignore();
}
