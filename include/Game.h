#pragma once


#include "World.h"
#include <chrono>

class Game {

	private:
		
	World world;
	long frame;
	int frame_count;
	
	public:

	Game();
	void run();
	void render();
    

	void start_simulation();
	void pause_simulation();
	void quit();
	void load_world();
	void save_world();
	void increase_fps();
	void decrease_fps();
	void draw_bridges();
	

};
