#pragma once
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "TextureLoader.h"
#include "Game.h"

class GameOverlay
{
public:
	 void addFonts(std::string path, std::string key, int fontSize );
	 TTF_Font* getFont(std::string key);
	
private:
	std::map<std::string, TTF_Font*> fonts;

};

class Label
{
	friend class GameOverlay;

public:
	Label();
	Label(std::string text, std::string font, int x, int y, SDL_Color& labelColor);
	~Label();
	void setLabelText(SDL_Renderer* renderer, std::string text, TTF_Font* font);
	void drawLabel(SDL_Renderer* renderer);


private:
	std::string text;
	std::string font;
	SDL_Rect position;
	SDL_Color color;
	SDL_Texture* texture;

};