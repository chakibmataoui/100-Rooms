#pragma once
#include "gl_include.h"
#include "Button.h"
#include "Text.h"
class Title
{
public:
	Title();
	~Title();
	void Start(ALLEGRO_DISPLAY *display);
	bool Initialize();
	void Update();
	void Draw();
	void Erase();
	double coef = 0.0;
	Button play;
	Button credits;
	Button options;
	Button exit;
	Rect size;
	Text version;
	ALLEGRO_BITMAP *sprite_sheet = NULL;
	ALLEGRO_BITMAP *BM_mouse = NULL;
	ALLEGRO_BITMAP *BM_title = NULL;
	ALLEGRO_BITMAP *doubleframe = NULL;
	ALLEGRO_BITMAP *frame = NULL;
	ALLEGRO_MOUSE_CURSOR *mouse_cursor = NULL;
	ALLEGRO_DISPLAY *cur_display = NULL;
};
