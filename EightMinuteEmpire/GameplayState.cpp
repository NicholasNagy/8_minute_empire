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
#include "PlayerStrategies.h"
#include "GameEngine.h"

using GraphWorld::Map;

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

bool spacePress = false;
bool gameEnd = false;
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

	// Set the statsObserver to observer the GameplayState
	StatsObserver* statsObserver = new StatsObserver(GameplayState::Instance());
	game->setStatsObserver(statsObserver);

	// start a new gameplay status
	startNewStatus(gameMessages);
	updateStatus("Press Space to engage card selection. '7' to change Strategy type");

	// loop over all the players and update their list of owned countries after the end of each turn.
	for (Player* player : game->players()) { player->updateListOfOwnedCountries(game); }
	updateStatistics(game->players());

	Hand* hand = new Hand(game->deck()); cout << "\n------------------------------------------------------------\n";
	game->setHand(hand);

	hand->printHand();
	cout << "\n------------------------------------------------------------\n";
	cout << endl <<gameMessages <<endl;


	
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
	string path = game->getMapLoader()->getTileSetPath();
	texture = TextureLoader::loadTexutre(path.c_str(), renderer);
	numCountries = Map::instance()->getNumCountries();
	Map::instance()->getTileMap()->drawTileMap(renderer, texture);
	startingCountry = Map::instance()->getStartingCountry();
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

	printf("\nGamePlay Paused");
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
			if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 1);		
				break;

			case SDLK_2:
				if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 2);

				break;
			case SDLK_3:
				if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 3);				
				break;
			case SDLK_4:
				if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 4);
				break;

			case SDLK_5:
				if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 5);
				break;
			case SDLK_6:
				if (!ActionState::inActionState && spacePress)
				handleCardSelection(game, 6);			
				break;
			case SDLK_7:
				if (!ActionState::inActionState && !gameEnd)
					if (ActionState::toPlay->getStrategy().compare("Human") == 0)
					{
						ActionState::toPlay->setStrategy(new  ModerateCPU());
						string userMessage = ActionState::toPlay->getName() + " status changed form Human to ModerateCPU.";
						updateStatus(userMessage);
						updateStatus("Press Space to engage card selection.");
						cout << endl << userMessage << endl;

					}
					else if (ActionState::toPlay->getStrategy().compare("ModerateCPU") == 0)
					{
						ActionState::toPlay->setStrategy(new  GreedyCPU());
						string userMessage = ActionState::toPlay->getName() + " status changed form ModerateCPU to GreedyCPU.";
						updateStatus(userMessage);
						updateStatus("Press Space to engage card selection.");
						cout << endl << userMessage << endl;


					}
					else if (ActionState::toPlay->getStrategy().compare("GreedyCPU") == 0)
					{
						ActionState::toPlay->setStrategy(new  Human());
						string userMessage = ActionState::toPlay->getName() + " status changed form GreedyCPU to Human.";
						updateStatus(userMessage);
						updateStatus("Press Space to engage card selection.");
						cout << endl << userMessage << endl;
					}
				break;

			case SDLK_SPACE:
				if (!gameEnd)
				{			
					spacePress = true;
					if (ActionState::toPlay->getStrategy().compare("GreedyCPU") == 0 || ActionState::toPlay->getStrategy().compare("ModerateCPU") == 0)
						handleCardSelection(game, 0);
					else
						updateStatus("Engaged Card Selection. Pick a card by pressing 1-6 on the keyboard");
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
	typeCol = mouse.x / GRID_CELL_SIZE;
	typeRow = mouse.y / GRID_CELL_SIZE;

	if (mouse.x < MAP_WIDTH * GRID_CELL_SIZE)
	{
		type = Map::instance()->getTileMap()->tiles[typeRow][typeCol];
		if (type < numCountries && type >= 0)
			hoveredCountry = Map::instance()->getCountry(type);
	}
}

void GameplayState::handleCardSelection(Game* game, int position)
{
	ActionState::toPlay->pickCard(game, position);
	ActionState::toPlay->updateGoodsPoints(ActionState::toPlay->getHand()->getGood());
	ActionState::toPlay->playCard(game);

}

void GameplayState::nextMove(Game* game)
{

	ActionState::inActionState = false;
	spacePress = false;
	ActionState::toPlay->computeScore();
	gameMessages.clear();


	ActionState::toPlay->setHand(nullptr);
	ActionState::toPlay->setCardToPlay(ActionState::toPlay->getCardsToPlay() - 1);

	// loop over all the players and update their list of owned countries after the end of each turn.
	for (Player* player : game->players()) { player->updateListOfOwnedCountries(game); }
	updateStatistics(game->players());

		//if there are no more remaining cards the game will end
		if (!checkRemainingCards(game))
			return;
	
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

		gameMessages = ActionState::toPlay->getName() + " (" + ActionState::toPlay->getStrategy() + ") turn to move.";;
		startNewStatus(gameMessages);
	}
	else
		gameMessages = ActionState::toPlay->getName() + " turn to move.";
		cout << gameMessages << endl;
		startNewStatus(gameMessages);

		updateStatus("Press Space to engage card selection. '7' to change Strategy type");

}


bool GameplayState::checkRemainingCards(Game* game)
{
	int count = 0;
	for (Player* p : game->players())
	{
		if (p->getCardsToPlay() == 0)
			count++;
	}
	//If all players have no more cards to player, compute score and end the game
	if (count == numPlayers)
	{
		computeFinalScore(game);
		gameEnd = true;
		game->pushState(GameEngine::Instance());
		return false;

	}
	return true;
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
			Map::instance()->getCountry(i)->updateOccupyingPlayerScore(0, p);
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
				cout << endl << gameMessages << endl;
				startNewStatus(gameMessages);
				return game->players().at(i);
			}				
		}		
	}
	else
	{
		gameMessages = "Game Over - TIE GAME !";
		cout << endl << gameMessages << endl;
		startNewStatus(gameMessages);
		return nullptr;
	}

}

// updates the statistics data member which is realized from the observable interface.
// It also calls the notify() method.
void GameplayState::updateStatistics(vector<Player*> players)
{
	string statsBarGraph;


	for (Player* p : players) {


		statsBarGraph = statsBarGraph + p->getName() + " (" + p->getStrategy() + ")" "\n";

		if (p->getCardsToPlay() == 0)
			statsBarGraph = statsBarGraph + "Congrats on finishing your turn" + "\n";

		statsBarGraph = statsBarGraph + "Owned Countries IDs: ";
		for (GraphWorld::Country* country : p->getListOfOwnedCountries())
		{
			statsBarGraph = statsBarGraph + to_string(country->getID()) + ",";
		}
		statsBarGraph = statsBarGraph + "\n";
		statsBarGraph = statsBarGraph + "Coins " + to_string(p->getMoney()) + "\n";
		statsBarGraph = statsBarGraph + "Cards " + to_string(p->getCardsToPlay()) + "\n";
		statsBarGraph = statsBarGraph + "Cities Left " + to_string(p->getNumCities()) + "\n";
		statsBarGraph = statsBarGraph + "Armies Left " + to_string(p->getArmies()) + "\n";
		statsBarGraph = statsBarGraph + drawSingleColumn("Owned Countries", p->getNumOfOwnedCountries());
		statsBarGraph = statsBarGraph + drawSingleColumn("Owned Goods", p->getNumOfGoods());
		statsBarGraph = statsBarGraph + drawSingleColumn("Victory Points", p->getTotalVictoryPoints());
	
		statsBarGraph = statsBarGraph + "\n";
	}


	setStatitics(statsBarGraph);
	notify();
}

string GameplayState::drawSingleColumn(string colName, int colSize)
{
	string col = colName + " ";
	for (int i = 0; i < colSize; i++)
		col = col + "=";
	col = col + "\n";
	return col;
}


void GameplayState::draw(Game* game)
{
	SDL_RenderClear(renderer);
	Map::instance()->getTileMap()->drawTileMap(renderer, texture);

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
	//playerInfoLabel->setLabelText(renderer, screen, ss.str(), ui.getFont("unispace bd"));
	ss.clear();

	playerInfoLabel->setLabelText(renderer, screen, game->statsObserver()->getStatistics() , ui.getFont("unispace bd"));


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

