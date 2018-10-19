#include "../include/Game.h"

int main(int argc,char **argv) {
	al_init();
	al_init_image_addon();
	Game gm;
	gm.StartGame();
	return 0;
}