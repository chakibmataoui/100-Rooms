#include "..\include\Title.h"



Title::Title() : play("Play",{100,100,0,0})
{

}


Title::~Title()
{
}

void Title::Start(ALLEGRO_DISPLAY *display) {
	cur_display = display;
	double fps = FPS;
	clock_t previous, current, delta;
	previous = clock();
	Initialize();
	cout << "Title screen initialized" << endl;
	while (1) {
		current = clock();
		delta = current - previous;
		do {
			delta -= clock() - current;
			Update();
		} while (delta / CLOCKS_PER_SEC / 1000 > 1000 / fps);
		Draw();
		previous = current;
	}
	Erase();
}
bool Title::Initialize() {
	
	sprite_sheet = al_load_bitmap("ui_title.png");
	if (sprite_sheet == NULL) {
		cout << "Error while loading assets" << endl;
		return false;
	}
	BM_title = al_create_sub_bitmap(sprite_sheet,20,0,256-20,370);
	doubleframe = al_create_bitmap(WIDTH, HEIGHT);
	frame = al_get_target_bitmap();
	//Mouse routines
	al_install_mouse();
	BM_mouse = al_create_sub_bitmap(sprite_sheet,0,0,20,20);
	mouse_cursor  = al_create_mouse_cursor(BM_mouse, 0, 0);
	if (mouse_cursor == NULL) {
		cout << "Error while creating mouse" << endl;
		return false;
	}
	//al_hide_mouse_cursor(cur_display);
	al_set_mouse_cursor(cur_display, mouse_cursor);
	return true;
} 

void Title::Update() {
	
}

void Title::Draw() {
	al_set_target_bitmap(doubleframe);
	al_clear_to_color({ 0,0,0,1 });
	al_draw_scaled_bitmap(BM_title, 0, 0, al_get_bitmap_width(BM_title),al_get_bitmap_height(BM_title) 
		,(WIDTH/2) - (al_get_bitmap_width(BM_title)*2.5)/2, (HEIGHT/ 2) - (al_get_bitmap_height(BM_title)*2.5) / 2, al_get_bitmap_width(BM_title)*2.5, al_get_bitmap_height(BM_title)*2.5,0);
	al_set_target_bitmap(frame);
	al_draw_scaled_bitmap(doubleframe, 0, 0, WIDTH, HEIGHT, 0, 0, al_get_display_width(cur_display), al_get_display_height(cur_display), 0);
	al_flip_display();
}
 

void Title::Erase() {
	al_destroy_bitmap(sprite_sheet);
}