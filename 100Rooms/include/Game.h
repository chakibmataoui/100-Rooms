#pragma once
#include "gl_include.h"
#include "Title.h"
#include "Game_Screen.h"
typedef enum { TITLE_SCREEN = 1, GAME_SCREEN, LOADING } Screen_State;
class Game
{
public:
	Game();
	void StartGame();
	void Initialize();
	~Game();
private:
	ALLEGRO_DISPLAY *main_window;
	Screen_State current_screen;
	Title title_screen;
	Game_Screen game_screen;
};

