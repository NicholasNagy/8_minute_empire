#include "ActionState.h"

PlaceNewArmiesState PlaceNewArmiesState::mPlaceNewArmiesState;
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

void ActionState::getClickedCountry(Game* game)
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
		ActionState::getClickedCountry(game);
		break;

	case SDL_KEYDOWN:
	{
		switch (GameplayState::event.key.keysym.sym)
		{
		case SDLK_RETURN:
			executeAction();
			game->popState();
			selectedCountries.clear();
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
	for (GraphWorld::Country* c : selectedCountries)
	{
		ActionState::toPlay->PlaceNewArmies(1, c);

	}

}

