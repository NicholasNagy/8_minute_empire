#include "GameplayState.h"
#include "TextureLoader.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <sstream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "Map.h"
#include "Bid.h"
#include "GameOverlay.h"
#include "Action.h"

GameplayState GameplayState::mGameplayState;
SDL_Renderer* GameplayState::renderer = nullptr;

SDL_Texture* texture = nullptr;
SDL_Surface* screen;
SDL_Rect cursor;
SDL_Rect cursorShadow = { cursor.x, cursor.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
SDL_Color cursorShadowColor = { 107, 107, 107, 107 };
bool mouseActive = false;
bool mouseHover = false;
bool mouseClick = false;
bool bid = false;


GraphWorld::Map* gameMap;
GraphWorld::Country* hoveredCountry = nullptr;
GraphWorld::Country* srcCountry = nullptr;
GraphWorld::Country* destCountry = nullptr;
GraphWorld::Country* startingCountry = nullptr;

std::vector<GraphWorld::Country*> selectedCountries; //the countries selected by the use
int selectedAction = -1;  // the action id;
 
int numCountries;
int numPlayers;
Player* toPlay;

bool destroyArmyAction = false;
Player* toDestroy;

GameOverlay ui;
Label* countryHoverLabel;
Label* playerInfoLabel;
Label* gameMessagesLabel;
std::string gameMessages;
Label* cardsLabel;

int playerMove;  //The current player's turn

void GameplayState::init(Game* game)
{
	std::cout << "\nGame Started\n\n-------------------------------------------------\n\n";
	numPlayers = game->players().size();

	GameplayState::initWindow(game);

	GameplayState::initMap(game);

	GameplayState::initUI(game);

	//Bidding
	playerMove = Bid::initiateBidding(game);
	bid = true;

	//Initial Army Placement
	placeStartingArmies(game);

	//Deck shuffling
	game->deck()->shuffleDeck();
	game->deck()->printDeck();
	
	toPlay = game->players().at(playerMove);
	gameMessages =  toPlay->getName() + " turn to move. Select a card by pressing (1-6) on the keyboard. 'Enter' to confirm move.\n";

	Hand* hand = new Hand(game->deck()); cout << "\n------------------------------------------------------------\n";
	game->setHand(hand);

	hand->printHand();
	cout << "\n------------------------------------------------------------\n";
	cout << endl <<gameMessages;
}

void GameplayState::initWindow(Game* game)
{

	//Creating Game Window
	SDL_Window* gameWindow = SDL_CreateWindow("Eight Minute Empire", 0, SDL_WINDOWPOS_CENTERED, WINDOW_X, WINDOW_Y, SDL_WINDOW_SHOWN);
	game->setWindow(gameWindow);
	screen = SDL_GetWindowSurface(gameWindow);
	//Creating Renderer to draw on window
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, WINDOW_X, WINDOW_Y);

}

void GameplayState::initMap(Game* game)
{
	gameMap = game->getMap();
	string path = game->getMapLoader()->getTileSetPath();
	texture = TextureLoader::loadTexutre(path.c_str(), renderer);
	numCountries = gameMap->getNumCountries();
	gameMap->getTileMap()->drawTileMap(renderer, texture);
	startingCountry = gameMap->getStartingCountry();
	initPlayerHoldings(game);
}

void GameplayState::initUI(Game* game)
{
	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init: %s\n" << TTF_GetError();
		exit(2);
	}
	string bidding = "Biding initiated in console!";
	ui.addFonts("assets/Fonts/unispace bd.ttf", "unispace bd", 9);
	ui.addFonts("assets/Fonts/arial.ttf", "arial", 22);
	ui.addFonts("assets/Fonts/ARIALN.TTF", "arialN", 26);


	SDL_Color black = { 0,0,0,0 };
	SDL_Color r = {3,2,23,0 };

	countryHoverLabel = new Label(bidding, "arial", 0, 0, black);
	countryHoverLabel->setLabelText(renderer,screen, bidding, ui.getFont("arial"));
	countryHoverLabel->drawLabel(renderer);

	playerInfoLabel = new Label("", "unispace bd", 1040, 10, black);
	playerInfoLabel->setLabelText(renderer,screen, "", ui.getFont("unispace bd"));
	playerInfoLabel->drawLabel(renderer);

	gameMessagesLabel = new Label("", "arialN", 5, 585, black);
	gameMessagesLabel->setLabelText(renderer, screen, "", ui.getFont("arialN"));
	gameMessagesLabel->drawLabel(renderer);

	cardsLabel = new Label("", "unispace bd", 1035, 565, r);
	cardsLabel->setLabelText(renderer, screen, "", ui.getFont("unispace bd"));
	cardsLabel->drawLabel(renderer);

	SDL_RenderPresent(renderer);
	countryHoverLabel->destroyLabelTexture();
	playerInfoLabel->destroyLabelTexture();
	gameMessagesLabel->destroyLabelTexture();
	cardsLabel->destroyLabelTexture();

}

void GameplayState::pause()
{

	printf("Game paused");
}

void GameplayState::resume()
{
	printf("Game resumed");
}

void GameplayState::clean(Game* game)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(game->getWindow());
	SDL_Quit();
	std::cout << "Game State Cleaned";
}

void GameplayState::handleEvents(Game* game)
{

	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		game->setRunning(false);
		break;
	case SDL_MOUSEBUTTONDOWN:
		cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;

		if (bid)
		{

			if (selectedAction >= 0 && selectedAction != 1)
			{
				getClickedCountry(false);
			}
			else if (selectedAction == 1)
			{
				getClickedCountry(true);
			}
		}
		break;
	case SDL_MOUSEMOTION:
		cursor.x = cursorShadow.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		cursor.y = cursorShadow.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		//std::cout << "x: " << cursor.x << " y: " << cursor.y << std::endl;
		if (!mouseActive)
			mouseActive = true;
		if (bid)
			getHoveredCountry();
		break;
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouseHover)
			mouseHover = true;
		else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouseHover)
			mouseHover = false;
		break;

	case SDL_KEYDOWN:
		{

			switch (event.key.keysym.sym)
			{

			case SDLK_1:
				if (!destroyArmyAction)
				{
					 cout << " -- Selected Handslot 1 -- \n";
					 if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(1))
					 {
						 toPlay->setHand(game->hand()->getCardAtPosition(1, game->deck()));
						 toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(1));
						 toPlay->getHand()->printCard();
						 selectedAction = toPlay->getHand()->getAction()->getID();
					 }
					 else
						 cout << "\nCannot afford the cost of this card. Please select another card.\n";
				}
				else if (numPlayers <= 5)
				{
					toDestroy = game->players().at(0);
					cout << "Selected Player " << 1 << endl;
				}
				break;

			case SDLK_2:
				if (!destroyArmyAction)
				{
					cout << "-- Selected Handslot 2 -- \n";
					if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(2))
					{
						toPlay->setHand(game->hand()->getCardAtPosition(2, game->deck()));
						toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(2));
						selectedAction = toPlay->getHand()->getAction()->getID();
						toPlay->getHand()->printCard();
						cout << "selectedAction: " << selectedAction << endl;
					}
					else
						cout << "\nCannot afford the cost of this card. Please select another card.\n";

				}
				else if (numPlayers <= 5)
				{
					toDestroy = game->players().at(1);
					cout << "Selected Player " << 2 << endl;
				}
					
				break;
			case SDLK_3:
				if (!destroyArmyAction)
				{
					cout << "-- Selected Handslot 3 -- \n";
					if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(3))
					{
					toPlay->setHand(game->hand()->getCardAtPosition(3, game->deck()));
					toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(3));
					selectedAction = toPlay->getHand()->getAction()->getID();
					toPlay->getHand()->printCard();
					cout << "selectedAction: " << selectedAction << endl;
					}
					else
						cout << "\nCannot afford the cost of this card. Please select another card.\n";
					
				}
				else if (numPlayers <= 5 && numPlayers >=3 )
				{
					toDestroy = game->players().at(2);
					cout << "Selected Player " << 3 << endl;
				}
					
				break;
			case SDLK_4:
				if (!destroyArmyAction)
				{
					cout << "-- Selected Handslot 4 -- \n";
					if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(4))
					{
						toPlay->setHand(game->hand()->getCardAtPosition(4, game->deck()));
						toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(4));
						destroyArmyAction = true;
						toPlay->getHand()->printCard();
						selectedAction = toPlay->getHand()->getAction()->getID();
					}
					else
						cout << "\nCannot afford the cost of this card. Please select another card.\n";

				}
				else if (numPlayers == 4 || numPlayers == 5)
				{
					toDestroy = game->players().at(3);
					cout << "Selected Player " << 4 << endl;
				}

				break;

			case SDLK_5:
				if (!destroyArmyAction)
				{
					cout << "-- Selected Handslot 5 -- \n";
					if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(5))
					{
						toPlay->setHand(game->hand()->getCardAtPosition(5, game->deck()));
						toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(5));
						toPlay->getHand()->printCard();
						selectedAction = toPlay->getHand()->getAction()->getID();
					}
					else
						cout << "\nCannot afford the cost of this card. Please select another card.\n";

				}
				else if (numPlayers == 5)
				{
				toDestroy = game->players().at(4);
				cout << "Selected Player " << 5 << endl;
				}
				break;
			case SDLK_6:
				if (!destroyArmyAction)
				{
					cout << "-- Selected Handslot 6 -- \n";
					if (toPlay->getMoney() >= game->hand()->getCardCostAtPosition(6))
					{
						toPlay->setHand(game->hand()->getCardAtPosition(6, game->deck()));
						toPlay->setCoinPurse(toPlay->getMoney() - game->hand()->getCardCostAtPosition(6));
						toPlay->getHand()->printCard();
						selectedAction = toPlay->getHand()->getAction()->getID();
					}
					else
						cout << "Cannot afford the cost of this card. Please select another card.\n";
				}
				break;
			case SDLK_RETURN:

				if (selectedAction >= 0)
				{
					handlePlayerAction(game);
					nextMove(game);
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

void GameplayState::getHoveredCountry()
{
	static int typeCol;
	static int typeRow;
	static int type;
	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = gameMap->getTileMap()->tiles[typeRow][typeCol];
		if (type < numCountries && type >= 0)
			hoveredCountry = gameMap->getCountry(type);
	}
}

void GameplayState::getClickedCountry(bool armyMove)
{
	//Find out type of tile clicked
	static int typeCol;
	static int typeRow;
	static int type;

	typeCol = cursor.x / GRID_CELL_SIZE;
	typeRow = cursor.y / GRID_CELL_SIZE;

	if (cursor.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = gameMap->getTileMap()->tiles[typeRow][typeCol];
		GraphWorld::Country* clickedON = nullptr;
		if (type < numCountries && type >= 0)
			clickedON = gameMap->getCountry(type);

		if (!armyMove)
		{		
			selectedCountries.push_back(clickedON);
			std::cout << "\nSelected Country: " << clickedON->getID() << std::endl;			
		}
		else
		{
			switch (selectedCountries.size())
			{
			case 0:
				selectedCountries.push_back(clickedON);
				std::cout << "\nSelected Country To Move From: " << clickedON->getID() << std::endl;
				break;
			case 1:
				selectedCountries.push_back(clickedON);
				std::cout << "\nSelected Country To Move To: " << clickedON->getID() << std::endl;
				break;
			case 2:
				selectedCountries.at(1) = clickedON;
				std::cout << "\nSelected Country To Move To: " << clickedON->getID() << std::endl;
				break;
			}			
		}
	}
}

void GameplayState::handlePlayerAction(Game* game)
{

	switch (selectedAction)
	{
	case 0:
		handlePlaceNewArmies(game);
		break;
	case 1:
		handleMoveArmies(game);
		break;
	case 2:
		handleBuildCity(game);
		break;
	case 3:
		handleDestroyArmy(game);
		break;
	case 4: //and
		handleAndOrAction(game);
		break;
	case 5:  //or
		handleAndOrAction(game);
		break;
	case 6:
		handleIgnore(game);
		break;
	default:
		break;
	}

}

void GameplayState::handlePlaceNewArmies(Game* game)
{


		for (GraphWorld::Country* c : selectedCountries)
		{
			toPlay->PlaceNewArmies(1, c);
		
		}
		
	

}

void GameplayState::handleMoveArmies(Game* game)
{

	if (selectedCountries.size() == 2)
	{
		toPlay->MoveArmies(gameMap, selectedCountries[0], selectedCountries[1]);
	}


}

void GameplayState::handleBuildCity(Game* game)
{
	if (selectedCountries[0])
	{
		toPlay->BuildCity(selectedCountries.back());
	}


}

void GameplayState::handleDestroyArmy(Game* game)
{
	if(toDestroy)
	toPlay->DestroyArmy(toDestroy, selectedCountries.back());
}

void GameplayState::handleAndOrAction(Game* game)
{
}

void GameplayState::handleIgnore(Game* game)
{
	cout << "\nCard Ignored\n";
}





void GameplayState::nextMove(Game* game)
{
	toPlay->computeScore(gameMap);
	selectedAction = -1;
	selectedCountries.clear();
	destroyArmyAction = false;
	gameMessages.clear();
	toPlay->setHand(nullptr);
	toPlay->setCardToPlay(toPlay->getCardsToPlay() - 1); 

	if (game->players().at(numPlayers - 1)->getCardsToPlay() == 0)
	{

		cout << "Game Over!" << endl;
		computeFinalScore(game);
		return;
	}


	playerMove++;
	if (playerMove == numPlayers)
		playerMove = 0;

	toPlay = game->players().at(playerMove);
	gameMessages.clear();
	cout << "Next Hand..." << endl << gameMessages << endl;
	game->hand()->printHand();
	cout << "------------------------------------------------------------\n";
	gameMessages = toPlay->getName() + " turn to move. Select a card by pressing (1-6) on the keyboard. 'Enter' to confirm move.\n";
	cout << gameMessages;
}

void GameplayState::placeStartingArmies(Game* game)
{
	for ( Player* p: game->players() )
	{

		startingCountry->updateOccupyingPlayerScore(3, p);
		p->holdings().emplace(startingCountry->getID(), new Holdings());	
		p->PlaceNewArmies(3, startingCountry);

	}
	cout << "\n------------------------------------------------------------\n";
	


}

void GameplayState::initPlayerHoldings(Game* game)
{
	for (int i = 0; i < numCountries; i++)
	{
		for (Player* p : game->players())
		{
			gameMap->getCountry(i)->updateOccupyingPlayerScore(0, p);
			p->holdings().emplace(i, new Holdings());

		}

	}

}

Player* GameplayState::computeFinalScore(Game* game)
{
	std::vector<int> playerScores;
	for (Player* p : game->players())
	{
		playerScores.push_back(p->getVictoryPoints());

	}
	std::sort(playerScores.begin(), playerScores.end(), std::greater<int>());

	if (playerScores.at(0) > playerScores.at(1))
	{
		gameMessages = "Game Over -" + game->players().at(0)->getName() + " won!\n";
		return game->players().at(0);
	}
	else
	{
		gameMessages = "Game Over - TIE GAME !";
		return nullptr;
	}
	

	
}

void GameplayState::draw(Game* game)
{
	SDL_RenderClear(renderer);
	gameMap->getTileMap()->drawTileMap(renderer, texture);
	if (mouseActive && mouseHover)
	{
		SDL_SetRenderDrawColor(renderer, cursorShadowColor.r, cursorShadowColor.g, cursorShadowColor.b, cursorShadowColor.a);
		SDL_RenderFillRect(renderer, &cursorShadow);
	}

	countryHoverLabel->drawLabel(renderer);
	playerInfoLabel->drawLabel(renderer);
	gameMessagesLabel->drawLabel(renderer);
	cardsLabel->drawLabel(renderer);
	SDL_RenderPresent(renderer);

}

void GameplayState::update(Game* game)
{

	std::stringstream ss;
	countryHoverLabel->destroyLabelTexture();
	playerInfoLabel->destroyLabelTexture();
	gameMessagesLabel->destroyLabelTexture();
	cardsLabel->destroyLabelTexture();

	for (Player* p : game->players())
		ss << *p;

	playerInfoLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("unispace bd"));

	ss.clear();
	if (hoveredCountry)
	{
		std::stringstream ss;
		ss << *hoveredCountry << *toPlay->getHoldings(hoveredCountry);
		countryHoverLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("arial"));

	}
	ss.clear();
	gameMessagesLabel->setLabelText(renderer, screen, gameMessages, ui.getFont("arialN"));

	cardsLabel->setLabelText(renderer, screen, "1\n2\n3\n4\n5\n6\n", ui.getFont("unispace bd"));



}