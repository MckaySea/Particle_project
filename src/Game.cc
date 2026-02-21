#include "Game.h"
#include "/public/colors.h"

//just initilizing the world with temp dimensions, should be changed
Game::Game() : world(50, 50) {

}

//this is what main.cc is calling and will be our infinite game loop eventually
void Game::run() {
	bool is_running = true;
	bool is_paused = true;
	set_raw_mode(true);
	set_cursor_mode(false);
	clearscreen();
	while(is_running){
		int input = quick_read();
		if(input == 'q' || input == 'Q'){
			is_running = false;
		} else if(input == 'p' || input == 'P'){
			is_paused = !is_paused;
		}
		if(!is_paused){
		frame_count++;
		}
 }

}
