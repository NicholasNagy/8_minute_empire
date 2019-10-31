#include "TextureLoader.h"

SDL_Texture* TextureLoader::loadTexutre(const char* texture, SDL_Renderer* renderer)
{
	SDL_Surface* surf = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	return tex;
}

void TextureLoader::draw(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(renderer, texture, &src, &dest);
}
