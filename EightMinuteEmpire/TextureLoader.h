#pragma once
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"

class TextureLoader
{
public:
	static SDL_Texture* loadTexutre(const char* fileName, SDL_Renderer* renderer);
	static void draw(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);


};