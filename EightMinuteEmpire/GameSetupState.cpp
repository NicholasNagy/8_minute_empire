#include "GameSetupState.h"
#include "GameplayState.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Game.h"
#include "MapLoader.h"
#include "Map.h"
#include "Player.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"

GameSetupState GameSetupState::mGameSetupState;
SDL_Renderer* GameSetupState::renderer = nullptr;
std::vector<bool> GameSetupState::players = { true, true, false, false, false };
std::vector<int> GameSetupState::ages = { 18, 18, 18, 18, 18 };
bool GameSetupState::mapLoaded = false;

const int TWO_PLAYER_COIN_PURSE = 14;
const int THREE_PLAYER_COIN_PURSE = 11;
const int FOUR_PLAYER_COIN_PURSE = 9;
const int FIVE_PLAYER_COIN_PURSE = 8;

void GameSetupState::init(Game* game)
{
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, 800, 640);
}

void GameSetupState::clean(Game* game)
{
	ImGuiSDL::Deinitialize();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(game->getWindow());
	;	ImGui::DestroyContext();
	std::cout << "Game Setup State Cleaned\n";
}

void GameSetupState::pause()
{
	printf("Main menu paused\n");
}

void GameSetupState::resume()
{
	printf("Main menu resumed\n");
}

void GameSetupState::handleEvents(Game* game)
{
	ImGuiIO& io = ImGui::GetIO();
	int wheel = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			game->setRunning(false);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				io.DisplaySize.x = static_cast<float>(event.window.data1);
				io.DisplaySize.y = static_cast<float>(event.window.data2);
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				wheel = event.wheel.y;
			}
		default:
			break;
		}
	}
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	handleMapPicker(game);
	handlePlayerPicker(game);
	handleGameStart(game);

}

void GameSetupState::update(Game* game)
{
}

void GameSetupState::draw(Game* game)
{

	SDL_RenderClear(renderer);
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void GameSetupState::handleMapPicker(Game* game)
{

	ImGui::Begin("Choose your map", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImVec2 windowSize(300, 500);
	ImGui::SetWindowSize(windowSize);
	ImVec2 windowPos(5, 20);
	ImGui::SetWindowPos(windowPos);

	std::vector<std::string> maps = MapLoader::getInstalledMaps();
	static int selected = -1;
	for (int n = 0; n < maps.size(); n++)
	{
		char  buffer[200];
		const char* map = maps.at(n).c_str();
		sprintf_s(buffer, "%s", map);
		if (ImGui::Selectable(buffer, selected == n))
		{
			selected = n;
			MapLoader::selectedMap = maps.at(n);
			std::cout << "User selected: " << maps.at(n) << std::endl;
		}
	}

	ImVec2 buttonSize(100, 50);
	static bool load = false;
	if (ImGui::Button("Load this Map", buttonSize))
	{
		if (selected != -1)
			load = initMapLoader(game);
	}
	if (load & 1)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Map Succesfully Loaded.");
	else
		ImGui::Text("Check console for Map validation.");

	ImGui::End();

}

void GameSetupState::handlePlayerPicker(Game* game)
{
	ImGui::Begin("Choose Players", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImVec2 windowSize(490, 500);
	ImGui::SetWindowSize(windowSize);
	ImVec2 windowPos(305, 20);
	ImGui::SetWindowPos(windowPos);

	ImGui::Columns(3, "mixed");
	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
	ImGui::Button("Player 1");
	ImGui::PopStyleColor(3);
	static int age1 = ages[0];
	ImGui::SliderInt("age1", &age1, 0, 100);
	ImGui::Text("");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player 1 playing");
	ImGui::NextColumn();


	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
	ImGui::Button("Player 2");
	ImGui::PopStyleColor(3);
	static int age2 = ages[1];
	ImGui::SliderInt("age2", &age2, 0, 100);
	ImGui::Text("");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player 2 playing");
	ImGui::NextColumn();

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
	ImGui::Button("Player 3");
	static int age3 = ages[2];
	ImGui::PopStyleColor(3);
	ImGui::SliderInt("age3", &age3, 0, 100);
	static bool status3 = players[2];
	if (ImGui::Button("Add/Remove Player 3"))
	{
		status3 = !status3;
		players[2] = status3;
	}
	if (status3 & 1)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player 3 added.");
	else
		ImGui::TextDisabled("Player 3 NOT playing");
	ImGui::NextColumn();


	ImGui::Separator();
	ImGui::Columns(3, "mixed");

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
	ImGui::Button("Player 4");
	ImGui::PopStyleColor(3);
	static int age4 = ages[3];
	ImGui::SliderInt("age4", &age4, 0, 100);
	static bool status4 = players[3];
	if (ImGui::Button("Add/Remove Player 4"))
	{
		status4 = !status4;
		players[3] = status4;
	}
	if (status4 & 1)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player 4 added.");
	else
		ImGui::TextDisabled("Player 4 NOT playing");
	ImGui::NextColumn();


	ImGui::Button("Player 5");
	static int age5 = ages[4];
	ImGui::SliderInt("age5", &age5, 0, 100);
	static bool status5 = players[4];
	if (ImGui::Button("Add/Remove Player 5"))
	{
		status5 = !status5;
		players[4] = status5;


	}
	if (status5 & 1)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Player 5 added.");
	else
		ImGui::TextDisabled("Player 5 NOT playing");
	ImGui::NextColumn();

	ImGui::Columns(1);
	ImGui::Separator();
	ages[0] = age1;
	ages[1] = age2;
	ages[2] = age3;
	ages[3] = age4;
	ages[4] = age5;



	ImGui::End();
}

void GameSetupState::handleGameStart(Game* game)
{
	bool select = false;
	ImGui::Begin("Start Game", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImVec2 windowSize(200, 66);
	ImGui::SetWindowSize(windowSize);
	ImVec2 windowPos(595, 520);
	ImGui::SetWindowPos(windowPos);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
	ImVec2 buttonSize(190, 50);
	if (ImGui::Button("START GAME!", buttonSize))
	{
		if (mapLoaded)
			select = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::End();
	if (select)
	{

		setupPlayers(game);
		game->changeState(GameplayState::Instance());
	}

}

bool GameSetupState::initMapLoader(Game* game)
{
	MapLoader* mapLoader = nullptr;
	mapLoaded = false;
	mapLoader = MapLoader::initiateMapPicker();

	if (mapLoader)
	{
		std::cout << *mapLoader << std::endl;
		GraphWorld::Map* map = new GraphWorld::Map(&mapLoader->getMapName(), mapLoader->getNumCountries(), mapLoader->getNumContinents());
		game->setMap(map);
		GraphWorld::TileMap* tileMap = new GraphWorld::TileMap();
		map->setTileMap(tileMap);
		game->setMapLoader(mapLoader);

		if (mapLoader->load(map, tileMap, MAP_HEIGHT, MAP_WIDTH))
		{
			map->printMap();
			std::cout << "Map Successfully loaded." << std::endl;
			mapLoaded = true;
			return true;
		}

		delete map;
		delete tileMap;
		delete mapLoader;

		return false;
	}
}

void GameSetupState::setupPlayers(Game* game)
{
	std::string name;
	int x = 1;
	for (int i = 0; i < players.size(); i++)
	{
		name = "Player " + to_string(x);
		if (players[i])
		{
			Player* p = new Player(&name, ages[i]);
			game->players().push_back(p);
			x++;
		}
	}
	assignCoins(game);

}

void GameSetupState::assignCoins(Game* game)
{

	std::vector players = game->players();
	const int numPlayers = players.size();

	switch (numPlayers)
	{
	case 2:
		game->players().at(0)->setCoinPurse(TWO_PLAYER_COIN_PURSE);
		game->players().at(1)->setCoinPurse(TWO_PLAYER_COIN_PURSE);
		break;
	case 3:
		game->players().at(0)->setCoinPurse(THREE_PLAYER_COIN_PURSE);
		game->players().at(1)->setCoinPurse(THREE_PLAYER_COIN_PURSE);
		game->players().at(2)->setCoinPurse(THREE_PLAYER_COIN_PURSE);
		break;
	case 4:
		game->players().at(0)->setCoinPurse(FOUR_PLAYER_COIN_PURSE);
		game->players().at(1)->setCoinPurse(FOUR_PLAYER_COIN_PURSE);
		game->players().at(2)->setCoinPurse(FOUR_PLAYER_COIN_PURSE);
		game->players().at(3)->setCoinPurse(FOUR_PLAYER_COIN_PURSE);
		break;
	case 5:
		game->players().at(0)->setCoinPurse(FIVE_PLAYER_COIN_PURSE);
		game->players().at(1)->setCoinPurse(FIVE_PLAYER_COIN_PURSE);
		game->players().at(2)->setCoinPurse(FIVE_PLAYER_COIN_PURSE);
		game->players().at(3)->setCoinPurse(FIVE_PLAYER_COIN_PURSE);
		game->players().at(4)->setCoinPurse(FIVE_PLAYER_COIN_PURSE);
		break;

	default:
		break;
	}





}
