#include "..\include\Game.h"



Game::Game()
{
	//INIT WINDOW SETTINGS 
	//WIDTH = 1920; HEIGHT = 1080;
	//Window initializations
	main_window = al_create_display(WIDTH, HEIGHT);
	al_set_window_position(main_window, 0, 0);
	al_set_display_flag(main_window, ALLEGRO_FRAMELESS, true);
	al_set_display_flag(main_window, ALLEGRO_FULLSCREEN_WINDOW, true);
	al_set_display_option(main_window, ALLEGRO_RENDER_METHOD, ALLEGRO_REQUIRE);
	al_set_window_title(main_window, "100 Rooms");

	//Game Initializations
	current_screen = GAME_SCREEN;
}

Game::~Game()
{
	al_destroy_display(main_window);
}

void Game::StartGame() {
	if (current_screen == TITLE_SCREEN) {
		cout << "Title screen loading" << endl;
		title_screen.Start(main_window);
	}

	if (current_screen == GAME_SCREEN) {
		cout << "Game screen loading" << endl;
		if (!game_screen.Start(main_window)) {
			exit(0);
		}
	}
	StartGame();
}

void Game::Initialize() {
	
}

