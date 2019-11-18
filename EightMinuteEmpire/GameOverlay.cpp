#include "GameOverlay.h"

void GameOverlay::addFonts(std::string path, std::string key, int fontSize)
{
	fonts.emplace(key, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* GameOverlay::getFont(std::string key)
{
	return fonts.at(key);
}


Label::Label()
{
}

Label::Label(std::string text, std::string font, int x, int y, SDL_Color& labelColor)
{
	this->text = text;
	this->font = font;
	position.x = x;
	position.y = y;
	this->color = labelColor;
}
Label::~Label()
{
	delete this;
}

void Label::setLabelText(SDL_Renderer* renderer, SDL_Surface* screen, std::string text, TTF_Font* font)
{
	this->text = text;
	SDL_Surface* s = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, MAP_WIDTH*GRID_CELL_SIZE);
	texture = SDL_CreateTextureFromSurface(renderer, s);
	SDL_BlitSurface(s, NULL, screen, &position);
	SDL_FreeSurface(s);
	SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
}

void Label::setLabelColor(SDL_Color c)
{
	color = c;
}

void  Label::drawLabel(SDL_Renderer* renderer)
{
	TextureLoader::draw(renderer,texture, nullptr, &position);
}

void Label::destroyLabelTexture()
{
	SDL_DestroyTexture(texture);
}

