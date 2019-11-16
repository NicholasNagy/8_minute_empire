#include "ActionState.h"

PlaceNewArmiesState PlaceNewArmiesState::mPlaceNewArmiesState;
MoveArmiesState MoveArmiesState::mMoveArmiesState;
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

void ActionState::getSelectedCountry(Game* game)
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

		}
	}


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
			game->popState();
			selectedCountries.clear();
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
