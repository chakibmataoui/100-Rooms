#pragma once
#include "gl_include.h"
class Button
{
public:
	Button();
	Button(string text,Rect size);
	Rect size;
	string text;
	ALLEGRO_BITMAP *btn_bitmap;
};

