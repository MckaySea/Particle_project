#include "Game.h"
#include "/public/colors.h"

//just initilizing the world with temp dimensions, should be changed
Game::Game() : world(20, 20) {
	auto [r, c] = get_terminal_size();
	world = world(r - 5, c);
	frame = 0;
	frame_count = 0;
}

//this is what main.cc is calling and will be our infinite game loop eventually
void Game::run() {
	bool is_running = true;
	bool is_paused = true;
	int target_fps = 5;

	set_raw_mode(true);
	set_cursor_mode(false);
	set_mouse_mode(true);

	static ParticleType current_spawn_type = ParticleType::WATER;

	on_mousedown(mousedown_handler);


	clearscreen();
	movecursor(world.getRows() / 2, world.getCols() / 2 - 15);
	cout << "Welcome to Particle Sim! Press any key to start.";
	while(quick_read() == ERR){
		usleep(10000);
	}

	while(is_running){
		auto start_time = chrono::steady_clock::now();
		int input = quick_read();
		if(input == 'q' || input == 'Q'){
			is_running = false;
		} else if(input == 'p' || input == 'P'){
			is_paused = !is_paused;
		} else if(input == 'f' || input == 'F'){ //speed up frame rate
			frame_count++;
		} else if (input == 's' || input == 'S'){ //slow down frame rate
			frame_count--;
			if(target_fps < 1)
				target_fps = 1;
		} else if (input == 'l' || input == 'L'){ // load the world from disk
			world.load("world_save.txt");
		} else if (input == 'm' || input == 'M' || input == 'w' || input == 'W'){ // save to disk
			world.save("world_save.txt");
		} else if (input == 'd' || input 'D'){
			draw_bridges();
		}

		if(!is_paused){
			if(world.alive_count() == 0){
				is_paused = true;
			} else {
				world.physics();
			}
			frame_count++;
		}
 }

}
