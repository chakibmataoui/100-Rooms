#pragma once
#include "gl_include.h"

//Game elements
typedef struct level_pos{ int x; int y; } level_pos;
typedef struct object { int x; int y; int w; int h; } object;
typedef enum player_state { NOMOVE, RUNNING, JUMPING } player_state;
typedef struct player : object { player_state state; float velocity_x; float velocity_y; ALLEGRO_BITMAP *player_sprite; bool direction_right = true; bool jumping = false; } player;
typedef enum door_state { CLOSE, OPEN } door_state;
typedef enum door_direction{UP,DOWN,LEFT,RIGHT} door_direction;
typedef struct door : object { door_state state = CLOSE; door_direction direction; static int s_w; static int s_h; } door;

//Game level

typedef struct room_position { int x; int y; } room_position;
typedef struct room {
	vector<door> doors; vector<object> platforms; room_position rp; char doors_bit = 0;
	void add_door(door d) {
		switch (d.direction) {
		case UP:
			if ((doors_bit & 0xC0) == 0) {
				doors.push_back(d);
				doors_bit += 0xC0;
			}
			break;
		case DOWN:
			if ((doors_bit & 0x30) == 0) {
				doors.push_back(d);
				doors_bit += 0x30;
			}
			break;
		case LEFT:
			if ((doors_bit & 0x0C) == 0) {
				doors.push_back(d);
				doors_bit += 0x0C;
			}
			break;
		case RIGHT:
			if ((doors_bit & 0x03) == 0) {
				doors.push_back(d);
				doors_bit += 0x03;
			}
			break;
		}
	}
} room;
typedef struct maze { room rooms[10][10]; room_position current_room; ALLEGRO_BITMAP *maze_buffer; /*Line X Collumn*/vector<int> keyrooms; ALLEGRO_BITMAP *room_bm; ALLEGRO_BITMAP *door_bm; } maze;
typedef struct camera { int x; int y; } camera;
class Game_Screen
{
public:
	Game_Screen();
	~Game_Screen();
	bool Start(ALLEGRO_DISPLAY *cur_display);
	bool Initialize();
	void Update();
	void Draw();
	void Erase();
	ALLEGRO_DISPLAY *gl_display;
	ALLEGRO_BITMAP *doubleframe, *frame, *sprite_sheet,/*TMP*/ *solid_rock;
	ALLEGRO_KEYBOARD_STATE ks;
	maze lvl_maze;
	camera main_cam;
	player pl;
	level_pos current_pos;
	object w1, w2, w3, w4;
	void create_maze();
};

static bool point_inside(int x, int y, object o);
static bool collide(object o1, object o2);

