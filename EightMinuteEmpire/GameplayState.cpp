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
bool bid = false;

GraphWorld::Map* gameMap;
GraphWorld::Country* hoveredCountry = nullptr;
GraphWorld::Country* startingCountry = nullptr;
 
int numCountries;
int numPlayers;

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
	gameMessages = ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move.";

	// Set the phaseObserver to observer the GameplayState
	PhaseObserver* phaseObserver = new PhaseObserver(GameplayState::Instance());
	game->setPhaseObserver(phaseObserver);

	// start a new gameplay status
	startNewStatus(gameMessages);
	

	Hand* hand = new Hand(game->deck()); cout << "\n------------------------------------------------------------\n";
	game->setHand(hand);

	hand->printHand();
	cout << "\n------------------------------------------------------------\n";
	cout << endl <<gameMessages <<endl;

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
	ui.addFonts("assets/Fonts/BRITANIC.TTF", "BRITANIC", 22);


	SDL_Color black = { 0,0,0,0 };
	SDL_Color r = {100,0,0,0 };

	countryHoverLabel = new Label(bidding, "arial", 0, 0, black);
	countryHoverLabel->setLabelText(renderer,screen, bidding, ui.getFont("arial"));
	countryHoverLabel->drawLabel(renderer);

	playerInfoLabel = new Label("", "unispace bd", 1040, 10, black);
	playerInfoLabel->setLabelText(renderer,screen, "", ui.getFont("unispace bd"));
	playerInfoLabel->drawLabel(renderer);

	gameMessagesLabel = new Label("", "arialN", 5, 545, black);
	gameMessagesLabel->setLabelText(renderer, screen, "", ui.getFont("BRITANIC"));
	gameMessagesLabel->drawLabel(renderer);

	cardsLabel = new Label("", "unispace bd", 1025, 555, r);
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


	case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{

			case SDLK_1:
			if (!ActionState::inActionState)				
				handleCardSelection(game, 1);		
				break;

			case SDLK_2:
				if (!ActionState::inActionState)
				handleCardSelection(game, 2);

				break;
			case SDLK_3:
				if (!ActionState::inActionState)
				handleCardSelection(game, 3);				
				break;
			case SDLK_4:
				if (!ActionState::inActionState)
				handleCardSelection(game, 4);
				break;

			case SDLK_5:
				if (!ActionState::inActionState)
				handleCardSelection(game, 5);
				break;
			case SDLK_6:
				if (!ActionState::inActionState)
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
	ActionState::toPlay->pickCard(game, position);
	//int selectedActionID = ActionState::toPlay->getHand()->getAction()->getID();
	//startNewStatus(ActionState::toPlay->getName() + " Has Selected Handslot " + to_string(position)  + "\n");
	ActionState::toPlay->playCard(game);

}


void GameplayState::nextMove(Game* game)
{
	ActionState::inActionState = false;
	bool cpu = false;
	ActionState::toPlay->computeScore(gameMap);
	gameMessages.clear();
	ActionState::toPlay->setHand(nullptr);
	ActionState::toPlay->setCardToPlay(ActionState::toPlay->getCardsToPlay() - 1);

	if (game->players().at(numPlayers - 1)->getCardsToPlay() == 0)
	{
		cout << "\nGame Over!" << endl;
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
		gameMessages = ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move.";;
		startNewStatus(ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move.");
	}
	else
		gameMessages = ActionState::toPlay->getName() + " turn to move. Select a card by pressing (1-6) on the keyboard.";
	cout << gameMessages << endl;
		startNewStatus(gameMessages);

	if(cpu)
		handleCardSelection(game, 0);
	
}

void GameplayState::placeStartingArmies(Game* game)
{
	for ( Player* p: game->players() )
	{
		ActionState::toPlay = p;
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
	countryHoverLabel->destroyLabelTexture();
	playerInfoLabel->destroyLabelTexture();
	gameMessagesLabel->destroyLabelTexture();
	cardsLabel->destroyLabelTexture();

	//Player stats
	std::stringstream ss;
	for (Player* p : game->players())
		ss << *p;
	playerInfoLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("unispace bd"));
	ss.clear();

	//Hovered country
	if (hoveredCountry)
	{
		std::stringstream ss;
		ss << *hoveredCountry << *ActionState::toPlay->getHoldings(hoveredCountry);
		countryHoverLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("arial"));

	}

	//Displaying Cards
	stringstream cards;
	int pos = 0;
	cards << "---------------CARDS IN HAND--------------" << endl;
	for (Card* card : game->hand()->cardsInHand())
	{
		cards << (pos + 1) << ".G=" << game->hand()->cardsInHand().at(pos)->getGood() << " " << 
			  game->hand()->cardsInHand().at(pos)->getAction()->actionString(false) << endl;
		pos++;
	}
	cards << "------------------------------------------";
	cardsLabel->setLabelText(renderer, screen, cards.str(), ui.getFont("unispace bd"));


	//cout << "\nObservable status: " << GameplayState::Instance()->getStatus() << "\nObserver Status: " << game->phaseObserver()->getStatus() << endl;

	//Displaying game messages
	gameMessagesLabel->setLabelText(renderer, screen, game->phaseObserver()->getStatus(), ui.getFont("BRITANIC"));

}

