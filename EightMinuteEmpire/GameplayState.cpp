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
#include "ActionState.h"

GameplayState GameplayState::mGameplayState;
SDL_Event GameplayState::event;
SDL_Renderer* GameplayState::renderer = nullptr;

SDL_Texture* texture = nullptr;
SDL_Surface* screen;
SDL_Rect mouse;
SDL_Rect cursorShadow = { mouse.x, mouse.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
SDL_Color cursorShadowColor = { 107, 107, 107, 107 };
bool mouseActive = false;
bool mouseHover = false;
bool inActionState = false;
bool bid = false;


GraphWorld::Map* gameMap;
GraphWorld::Country* hoveredCountry = nullptr;
GraphWorld::Country* srcCountry = nullptr;
GraphWorld::Country* destCountry = nullptr;
GraphWorld::Country* startingCountry = nullptr;

int selectedCardPos = -1;  // the action id;
 
int numCountries;
int numPlayers;

Player* toDestroy;

GameOverlay ui;
Label* countryHoverLabel;
Label* playerInfoLabel;
Label* gameMessagesLabel;
std::string gameMessages;
Label* cardsLabel;

int playerMove;  //The current player's turn
Game* nextM;

void GameplayState::init(Game* game)
{
	nextM = game;
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
	
	ActionState::toPlay = game->players().at(playerMove);
	gameMessages = ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move. \n";

	Hand* hand = new Hand(game->deck()); cout << "\n------------------------------------------------------------\n";
	game->setHand(hand);

	hand->printHand();
	cout << "\n------------------------------------------------------------\n";
	cout << endl <<gameMessages;

	if (ActionState::toPlay->getStrategy().compare("GreedyCPU") == 0 || ActionState::toPlay->getStrategy().compare("ModerateCPU") == 0)
		handleCardSelection(game, 0);

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

	printf("\nInitiated Card Action State");
}

void GameplayState::resume()
{
	printf("\nGameplay State Resumed");
	nextMove(nextM);
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
	SDL_WaitEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		game->setRunning(false);
		break;
	case SDL_MOUSEMOTION:
		mouse.x = cursorShadow.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
		mouse.y = cursorShadow.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
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

if (!inActionState)
	case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{

			case SDLK_1:
				
				handleCardSelection(game, 1);		
				
				break;

			case SDLK_2:
				handleCardSelection(game, 2);

				break;
			case SDLK_3:
				handleCardSelection(game, 3);				
				break;
			case SDLK_4:
				handleCardSelection(game, 4);
				break;

			case SDLK_5:
				handleCardSelection(game, 5);
				break;
			case SDLK_6:
				handleCardSelection(game, 6);			
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
	typeCol = mouse.x / GRID_CELL_SIZE;
	typeRow = mouse.y / GRID_CELL_SIZE;

	if (mouse.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = gameMap->getTileMap()->tiles[typeRow][typeCol];
		if (type < numCountries && type >= 0)
			hoveredCountry = gameMap->getCountry(type);
	}
}

void GameplayState::handleCardSelection(Game* game, int position)
{
	inActionState = true;

	ActionState::toPlay->pickCard(game, position);
	ActionState::toPlay->playCard(game);

	handlePlayerAction(game);

}

void GameplayState::handlePlayerAction(Game* game)
{

	switch (ActionState::toPlay->getHand()->getAction()->getID())
	{
	case 0:
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 1:
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 2:
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 3:
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 4: //and
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 5:  //or
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	case 6:
		game->pushState(PlaceNewArmiesState::Instance());
		break;
	default:
		break;
	}

}


void GameplayState::nextMove(Game* game)
{
	inActionState = false;
	bool cpu = false;
	ActionState::toPlay->computeScore(gameMap);
	gameMessages.clear();
	ActionState::toPlay->setHand(nullptr);
	ActionState::toPlay->setCardToPlay(ActionState::toPlay->getCardsToPlay() - 1);

	if (game->players().at(numPlayers - 1)->getCardsToPlay() == 0)
	{

		cout << "Game Over!" << endl;
		computeFinalScore(game);
		return;
	}


	playerMove++;
	if (playerMove == numPlayers)
		playerMove = 0;

	ActionState::toPlay = game->players().at(playerMove);
	gameMessages.clear();
	cout << "\nNext Hand..." << endl << gameMessages << endl;
	game->hand()->printHand();
	cout << "------------------------------------------------------------\n";
	if (ActionState::toPlay->getStrategy().compare("GreedyCPU") == 0 || ActionState::toPlay->getStrategy().compare("ModerateCPU") == 0)
	{
		cpu = true;
		gameMessages = ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move. \n";;
	}
	else
	gameMessages = ActionState::toPlay->getName() + " turn to move. Select a card by pressing (1-6) on the keyboard. 'Enter' to confirm move.\n";
	cout << gameMessages;
	if(cpu)
		handleCardSelection(game, 0);
	
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
		for (int i = 0; i < game->players().size(); i++)
		{
			if (game->players().at(i)->getVictoryPoints() == playerScores.at(0))
			{
				gameMessages = "Game Over -" + game->players().at(i)->getName() + " won!\n";
				return game->players().at(i);
			}
				
		}		
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
		ss << *hoveredCountry << *ActionState::toPlay->getHoldings(hoveredCountry);
		countryHoverLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("arial"));

	}
	ss.clear();
	gameMessagesLabel->setLabelText(renderer, screen, gameMessages, ui.getFont("arialN"));

	cardsLabel->setLabelText(renderer, screen, "1\n2\n3\n4\n5\n6\n", ui.getFont("unispace bd"));



}