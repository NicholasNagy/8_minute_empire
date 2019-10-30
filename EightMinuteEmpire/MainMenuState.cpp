#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "MainMenuState.h"
#include "GameSetupState.h"
#include "TextureLoader.h"
#include "Game.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"

MainMenuState MainMenuState::mMenuState;
SDL_Renderer* MainMenuState::renderer = nullptr;
SDL_Texture* MainMenuState::logoTexture = nullptr;

void MainMenuState::init(Game* game)
{
	renderer = SDL_CreateRenderer(game->getWindow(), -1, SDL_RENDERER_ACCELERATED);
	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, 800, 640);
	logoTexture = TextureLoader::loadTexutre("assets/logo.png", renderer);

	
}

void MainMenuState::clean(Game* game)
{
	ImGuiSDL::Deinitialize();
	SDL_DestroyRenderer(renderer);
	std::cout << "Menu State Cleaned\n";
}

void MainMenuState::pause()
{
	printf("Main menu paused");
}

void MainMenuState::resume()
{
	printf("Main menu resumed");
}

void MainMenuState::handleEvents(Game* game)
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
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	handleButtons(game);

}

void MainMenuState::update(Game* game)
{
}

void MainMenuState::draw(Game* game)
{

	SDL_RenderClear(renderer);
	SDL_Rect logo_pos = { 800 / 4, 100 / 4, 800 / 2, 640 / 2 };
	SDL_RenderCopy(renderer, logoTexture, NULL, &logo_pos);


	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void MainMenuState::handleButtons(Game* game)
{
	bool select = false;
	ImGui::Begin("Eight Minute Empire", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	ImVec2 windowSize(250, 120);
	ImGui::SetWindowSize(windowSize);
	ImVec2 windowPos(275, 325);
	ImGui::SetWindowPos(windowPos);
	ImVec2 size = ImGui::GetItemRectSize();
	ImGui::BeginGroup();
	ImVec2 buttonSize(250, 50);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
	if (ImGui::Button("NEW GAME", buttonSize))
		select = true;
	ImGui::PopStyleColor(3);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
	if (ImGui::Button("QUIT", buttonSize))
		game->setRunning(false);
	ImGui::PopStyleColor(3);
	ImGui::EndGroup();
	ImGui::End();

	if (select)
		game->changeState(GameSetupState::Instance());

}
