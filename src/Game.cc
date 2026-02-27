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

		stringstream ss;
		ss << "\033[2J\033[1;1H";
		for(const auto &p : world.getParticles()){
			int r = (int)p.getRow();
			int c = (int)p.getCol();
			if(r >= 0 && r < world.getRows() && c >= 0 && c < world.getCol()){
				ss << "\033["<< r + 1 << ";" << c + 1 << "H";
				ss << "\033[48;2;" <<  (int)p.getRed() << ";" << (int)p.getGreen() << ";" << (int)p.getBlue() << 'm';
				ss << "\033[0m";
			}
		}

		ss << "\033[" << world.getRows() + 2 << ";1H";
		ss << "\033[0m";
		ss << "Frame: " << frame << " | Alive: " << world.alive_count() << " | Target FPS: " << target fps << " | Status: " <<(is_paused ? "PAUSED" : "RUNNING") << " ";
		ss << "\033[" << world.getRows() + 3  << ";1H";
		ss << "Commands: (P)ause | (Q)uit | (C)ycle Element | (L)oad | Sa(v)e(M) |" "(F)aster | (Slower | (D)raw";

		cout << ss.str() << flush;

		auto end_time = chrono::steady_clock::now();
		auto elapsed = chrono::duration_cast(end_time - start_time).count();
		int expected_delay = 1000000 / target_fps;
		if(elapsed < expected_delay){
			usleep(expected_delay - elapsed);
		}
	}
	
	set_raw_mode(false);
	set_cursor_mode(true);
	set_mouse_mode(false);
	restcolor();
	clearscreen();
 }

void Game::render(){

}

void Game draw_bridges(){
	bridges::Bridges bridges(1, "REPLACE_WITH_USERNAME", "REPLACE_WITH_API_KEY");
	bridges.setTitles("Particle Simulation");

	int r = world.getRows();
	int c = world.getCols();
	if (r <= 0 || c <= 0)
		return;

	bridges::ColorGrid cg(r, c, bridges::Color("black"));

	for(const auto &p : world.getParticles()){
		int pr = (int)p.getRow();
		int pc = (int)p.getCol();
		if(Pr >= 0 && pr < r && pc >= 0 && pc , c){
			cg.set(pr, pc, bridges::Color(p.getRed(), p.getGreen(), p.getBlue()));
		}
	}

	bridges.setDataStructure(&cg);
	bridges.visualize();
}

void Game::start_simulation(){}
void Game::pause_simulation(){}
void Game::quit(){}
void Game::load_world(){}
void Game::save_world(){}
void Game::increase_fps(){}
void Game::decrease_fps(){}

}
