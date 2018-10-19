#include "..\include\Game_Screen.h"

int door::s_w = 0;
int door::s_h = 0;

Game_Screen::Game_Screen()
{
}


Game_Screen::~Game_Screen()
{
}

bool Game_Screen::Start(ALLEGRO_DISPLAY *cur_display) {
	gl_display = cur_display;
	double fps = FPS;
	clock_t previous, current, delta;
	previous = clock();
	if (!Initialize()) {
		cout << "Unable to initialize Game Screen" << endl;
		return false;
	}
	cout << "Game screen initialized" << endl;
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

bool Game_Screen::Initialize() {
	//Initialize double frames
	doubleframe = al_create_bitmap(WIDTH, HEIGHT);
	frame = al_get_target_bitmap();

	//Hide mouse
	al_hide_mouse_cursor(gl_display);

	//Configure keyboard
	al_install_keyboard();
	//Load assets
	//Load images
	pl.player_sprite = al_load_bitmap("gayaminim.png");
	lvl_maze.room_bm = al_load_bitmap("bg_screen.png");
	lvl_maze.door_bm = al_load_bitmap("door.png");
	solid_rock = al_load_bitmap("Rocks1.png");
	int wall_width = al_get_bitmap_width(al_load_bitmap("cadreOK.png"));
	if (pl.player_sprite == NULL || lvl_maze.room_bm == NULL) {
		cout << "Impossible to load ressources" << endl;
		return false;
	}
	//Load sound

	//Generate maze
	create_maze();
	current_pos = {0,lvl_maze.keyrooms[0] };
	door::s_w = al_get_bitmap_width(lvl_maze.door_bm);
	door::s_h = al_get_bitmap_height(lvl_maze.door_bm);
	/*Drawing of levels : 
	For each room we draw the entire screen in a 19200x10800 And we draw each room staticly but in the update we only update
	rooms that are in the camera view projection*/
	lvl_maze.maze_buffer = al_create_bitmap(WIDTH * 10, HEIGHT * 10);
	for (uint16_t i = 0; i < 10; ++i) {
		for (uint16_t j = 0; j < 10; ++j) {
			cout << "[";
			for (uint16_t r = 0; r < lvl_maze.rooms[i][j].doors.size(); ++r) {
				string dir;
				cout << lvl_maze.rooms[i][j].doors[r].direction << " ";
				lvl_maze.rooms[i][j].doors[r].w = door::s_w;
				lvl_maze.rooms[i][j].doors[r].h = door::s_h;
				if (lvl_maze.rooms[i][j].doors[r].direction == DOWN) {
					lvl_maze.rooms[i][j].doors[r].x = (WIDTH / 2 - door::s_w / 2) ;
					lvl_maze.rooms[i][j].doors[r].y = (HEIGHT - door::s_h - wall_width) ;
				}
				else if (lvl_maze.rooms[i][j].doors[r].direction == UP) {
					lvl_maze.rooms[i][j].doors[r].x = (WIDTH / 2 - door::s_w / 2) ;
					lvl_maze.rooms[i][j].doors[r].y = wall_width ;
				}
				else if (lvl_maze.rooms[i][j].doors[r].direction == RIGHT) {
					lvl_maze.rooms[i][j].doors[r].x = (WIDTH - door::s_w - wall_width) ;
					lvl_maze.rooms[i][j].doors[r].y = (HEIGHT / 2 - door::s_h / 2) ;
				}
				else {
					lvl_maze.rooms[i][j].doors[r].x = wall_width ;
					lvl_maze.rooms[i][j].doors[r].y = (HEIGHT / 2 - door::s_h / 2) ;
				}
			}
			cout << "]";
		}
		cout << endl;
	}
	/*Generate level physics :
	-Create walls of each room
	-Create the position of each door*/
	main_cam.x = current_pos.x * WIDTH;
	main_cam.y = 0;
	pl.w = al_get_bitmap_width(pl.player_sprite);
	pl.h = al_get_bitmap_height(pl.player_sprite);
	pl.x = 150;
	pl.y = HEIGHT - wall_width - pl.h;
	pl.state = player_state::NOMOVE;
	pl.velocity_x = 0;
	pl.velocity_y = 0;
	w1 = { 0 ,0,WIDTH,wall_width }, //Upper Wall
	w2 = { 0 ,0,wall_width,HEIGHT }, //Left
	w3 = { 0 ,HEIGHT- wall_width,WIDTH,wall_width }, //Bottom
	w4 = { WIDTH- wall_width ,0,wall_width,HEIGHT }; //Right

	return true;
}

void Game_Screen::Update() {
	al_get_keyboard_state(&ks);
	if (al_key_down(&ks, ALLEGRO_KEY_RIGHT) && pl.velocity_x <= 10) {
		pl.direction_right = true;
		pl.velocity_x += 5;
	}
	if (al_key_down(&ks, ALLEGRO_KEY_LEFT) && pl.velocity_x >= -10) {
		pl.direction_right = false;
		pl.velocity_x -= 5;
	}
	if (al_key_down(&ks, ALLEGRO_KEY_SPACE)) {
		if (pl.jumping == false) {
			pl.velocity_y = -24;
			pl.jumping = true;
		}
	}

	pl.x += pl.velocity_x;
	pl.y += pl.velocity_y;
	
	for (unsigned int i = 0; i < lvl_maze.rooms[current_pos.x][current_pos.y].doors.size(); ++i) {
		if (collide(pl, lvl_maze.rooms[current_pos.x][current_pos.y].doors[i])) {
			if (lvl_maze.rooms[current_pos.x][current_pos.y].doors[i].direction == LEFT)
				current_pos.y--;
			else if (lvl_maze.rooms[current_pos.x][current_pos.y].doors[i].direction == RIGHT)
				current_pos.y++;
			else if (lvl_maze.rooms[current_pos.x][current_pos.y].doors[i].direction == UP)
				current_pos.x--;
			else
				current_pos.x++;
			cout << current_pos.x << " " << current_pos.y << endl;
			pl.x = pl.h;
			pl.y = HEIGHT - w1.h - pl.h;
			pl.jumping = false;
			pl.state = player_state::NOMOVE;
			pl.velocity_x = 0;
			pl.velocity_y = 0;
		}
	}

	if (collide(pl, w3)) {
		//pl.y -= pl.velocity_y;
		pl.y = w3.y - pl.h;
		pl.velocity_y = 0;
		pl.jumping = false;
	}
	if (collide(pl, w1)) {
		//pl.y -= pl.velocity_y;
		pl.y = w1.y + w1.h;
		pl.velocity_y = -pl.velocity_y;
		//pl.jumping = false;
	}
	if (collide(pl, w2) || (collide(pl, w4))) {
		pl.x -= pl.velocity_x;
		pl.velocity_x = 0;
	}
	if(pl.velocity_x > 0)
		pl.velocity_x--;
	else if (pl.velocity_x < 0)
		pl.velocity_x++;
	if (pl.jumping)
		pl.velocity_y += 1;
	/*if (pl.velocity_y == 0)
		pl.jumping = false;*/
}

void Game_Screen::Draw() {
	al_set_target_bitmap(doubleframe);
	al_clear_to_color({ 0,0,0,1 });
	//BEGIN DRAW
	//DRAW BACKGROUND
	al_draw_bitmap(lvl_maze.room_bm, 0, 0, 0);
	//al_draw_bitmap_region(lvl_maze.maze_buffer, main_cam.x, main_cam.y,WIDTH,HEIGHT,0,0 ,0);
	//DRAW DOORS
	for (unsigned int i = 0; i < lvl_maze.rooms[current_pos.x][current_pos.y].doors.size(); ++i) { 
		al_draw_bitmap(lvl_maze.door_bm, lvl_maze.rooms[current_pos.x][current_pos.y].doors[i].x, lvl_maze.rooms[current_pos.x][current_pos.y].doors[i].y, 0);
	}
	//DRAW PLAYER
	al_draw_bitmap(pl.player_sprite, pl.x, pl.y, (pl.direction_right == true? 0: ALLEGRO_FLIP_HORIZONTAL ));
	//END DRAW
	al_set_target_bitmap(frame);
	al_draw_scaled_bitmap(doubleframe, 0, 0, WIDTH, HEIGHT, 0, 0, al_get_display_width(gl_display), al_get_display_height(gl_display), 0);
	al_flip_display();
}

void Game_Screen::Erase() {
	
}

void Game_Screen::create_maze() {
	//Create level
	/*Creation of level :
	We have a matrix of 10x10 rooms we start from the upper room to the lower room
	We choose for each row a random column which has not been selected before
	Than we create a random sequence between 2..9
	We create a path from 1 to each room in the sequence to 10 that should be the longest path to the end (horizental over vertical path)
	Than for each room in the matrix we randomly generate the number of doors and the direction of doors (if door is in the solution path
	creation of doors is randomly optional)
	*/
	//Generate a random maze sequence and random keyrooms sequence
	cout << "Maze creation" << endl;
	srand(time(0));
	vector<int> level_order;
	level_order.push_back(0);
	for (uint16_t i =  0; i < 10; ++i) {
		int r;
		do {
			r = rand() % 10;
		} while (find(lvl_maze.keyrooms.begin(), lvl_maze.keyrooms.end(), r) != lvl_maze.keyrooms.end());
		lvl_maze.keyrooms.push_back(r);
		if (i < 8) {
			int d;
			do {
				d = rand() % 8 + 1;
			} while (find(level_order.begin(), level_order.end(), d) != level_order.end());
			level_order.push_back(d);
		}
	}
	level_order.push_back(9);
	for (int i = 0; i < 10; ++i) {
		cout << "Keyroom = " << lvl_maze.keyrooms[i] << endl;
	}
	for (int i = 0; i < 10; ++i) {
		cout << "Level Solution[" << i << "] = " << level_order[i] << endl;
	}
	cout << "Maze created" << endl;
	//Generate solution path
	/*For path's doors generation we create a horizental than vetrtical path if the solution rooms are one above the other
	 and we generate a vertical than horizental path if the next room is under the current room*/
	uint16_t start, end;
	door r_door, l_door, u_door, d_door;
	r_door.direction = RIGHT;
	l_door.direction = LEFT;
	u_door.direction = UP;
	d_door.direction = DOWN;
	for (uint16_t i = 0; i < level_order.size() - 1; ++i) {
		//Case 1 the current room is above the next room
		if (level_order[i] < level_order[i + 1]) {
			//Horizental path creation
			//Left to right keyrooms (collumns)
			if (lvl_maze.keyrooms[level_order[i]] < lvl_maze.keyrooms[level_order[i + 1]]) {
				start = lvl_maze.keyrooms[level_order[i]];
				end = lvl_maze.keyrooms[level_order[i + 1]];
			}
			//Right to left keyrooms (collumns)
			else {
				start = lvl_maze.keyrooms[level_order[i + 1]];
				end = lvl_maze.keyrooms[level_order[i]];
			}
			for (uint16_t j = start; j <= end; ++j) {				
				if (j - start != 0) 
					lvl_maze.rooms[level_order[i]][j].add_door(l_door);
				if (j - end != 0) 
					lvl_maze.rooms[level_order[i]][j].add_door(r_door);
			}
			//Vertical path creation
			for (uint16_t j = level_order[i]; j <= level_order[i+1]; ++j) {
				if (j - level_order[i] != 0)
					lvl_maze.rooms[j][lvl_maze.keyrooms[level_order[i + 1]]].add_door(u_door);
				if (j - level_order[i+1] != 0)
					lvl_maze.rooms[j][lvl_maze.keyrooms[level_order[i + 1]]].add_door(d_door);
			}
		}
		//Case 2 the current room is under the next room
		else {
			//Vertical path creation
			for (uint16_t j = level_order[i+1]; j <= level_order[i]; ++j) {
				if (j - level_order[i + 1] != 0)
					lvl_maze.rooms[j][lvl_maze.keyrooms[level_order[i]]].add_door(u_door);
				if (j - level_order[i] != 0)
					lvl_maze.rooms[j][lvl_maze.keyrooms[level_order[i]]].add_door(d_door);
			}
			//Horizental path creation
			//Left to right keyrooms (collumns)
			if (lvl_maze.keyrooms[level_order[i]] < lvl_maze.keyrooms[level_order[i + 1]]) {
				start = lvl_maze.keyrooms[level_order[i]];
				end = lvl_maze.keyrooms[level_order[i + 1]];
			}
			//Right to left keyrooms (collumns)
			else {
				start = lvl_maze.keyrooms[level_order[i + 1]];
				end = lvl_maze.keyrooms[level_order[i]];
			}
			for (uint16_t j = start; j <= end; ++j) {
				if (j - start != 0)
					lvl_maze.rooms[level_order[i + 1]][j].add_door(l_door);
				if (j - end != 0)
					lvl_maze.rooms[level_order[i + 1]][j].add_door(r_door);
			}
		}
	}
}



static bool point_inside(int x, int y, object o) {
	if (x >= o.x && x <= o.x + o.w && y >= o.y && y <= o.y + o.h)
		return true;
	return false;
		
}

static bool collide(object o1, object o2) {
	if (point_inside(o1.x,o1.y,o2) || point_inside(o1.x + o1.w, o1.y, o2) || point_inside(o1.x + o1.w, o1.y + o1.h , o2) || point_inside(o1.x, o1.y + o1.h, o2))
		return true;
	return false;
}


