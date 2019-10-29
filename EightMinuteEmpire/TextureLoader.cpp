#include "TextureLoader.h"

SDL_Texture* TextureLoader::loadTexutre(const char* texture, SDL_Renderer* renderer)
{
	SDL_Surface* surf = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	return tex;
}
