#pragma once
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

class TextureLoader
{
public:
	static SDL_Texture* loadTexutre(const char* fileName, SDL_Renderer* renderer);


};