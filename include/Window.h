#ifndef Window_h
#define Window_h

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "./Block.h"
#include "./RushHour.h"

class Window {
public:

	SDL_Color target = {255, 0, 0};
	SDL_Color truck = {255, 255, 255};

	vector<string> menus;

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;

	const static int BLOCK_SIZE = 50;
	const static int MARGIN_SIZE = 8;

	bool quit = false;

	SDL_Renderer * renderer;
	TTF_Font * font;

    Window();

	void init();

	void render(RushHour & rh);

    void rect(int row, int col, int width, int height);

	void wall(int row, int col, bool orientation);

    void clear();

	void release();

	void menu();

	void text(float x, float y, string message);
};

#endif