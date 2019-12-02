#include "GameEngine.h"
#include "SDL.h"
#include "SDL_image.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"

GameEngine GameEngine::mGameEngine;
SDL_Renderer* GameEngine::renderer = nullptr;

void GameEngine::init(Game* game)
{
	for (int i = 0; i < 2; i++)
	{
		GameplayState::draw(game);
		GameplayState::update(game);
	}
	ImGui::CreateContext();
	ImGuiSDL::Initialize(GameplayState::renderer, WINDOW_X, WINDOW_Y);

}

void GameEngine::pause()
{
}

void GameEngine::resume()
{
}

void GameEngine::clean(Game* game)
{
}

void GameEngine::handleEvents(Game* game)
{
	GameplayState::handleEvents(game);
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
	ResultsTable(game);
}

void GameEngine::draw(Game* game)
{
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_RenderPresent(GameplayState::renderer);

}

void GameEngine::update(Game* game)
{

}

void GameEngine::ResultsTable(Game* game)
{
	ImGui::Begin("RESULTS TABLE", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	ImVec2 windowSize(600, 200);
	ImGui::SetWindowSize(windowSize);
	ImVec2 windowPos(WINDOW_X/4, WINDOW_Y/5);
	ImGui::SetWindowPos(windowPos);

		ImGui::Text("Results Table");
		ImGui::Columns(4, "mycolumns");
		ImGui::Separator();
		ImGui::Text("Player #"); ImGui::NextColumn();
		ImGui::Text("Cards"); ImGui::NextColumn();
		ImGui::Text("Victory Points"); ImGui::NextColumn();
		ImGui::Text("Coins"); ImGui::NextColumn();
		ImGui::Separator();

		static int selected = -1;
		for (int i = 0; i < game->players().size(); i++)
		{
			char label[32];
			sprintf_s(label, "%d", (i+1));
			if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
				selected = i;

			ImGui::NextColumn();
			//Cards
			ImGui::Text( to_string( game->players().at(i)->getCardsToPlay() ).c_str() ); ImGui::NextColumn();
			ImGui::Text( to_string( game->players().at(i)->getVictoryPoints() ).c_str() ); ImGui::NextColumn();
			ImGui::Text( to_string( game->players().at(i)->getMoney() ).c_str() ); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::End();

}
