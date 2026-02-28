#include "../include/Game.h"
#include "/public/colors.h"
#include <Bridges.h>
#include <ColorGrid.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

//just initilizing the world with temp dimensions, should be changed
Game::Game() : world(20, 20) {
	auto [r, c] = get_terminal_size();
	world = World(r - 5, c);
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

	auto mousedown_handler = [this, &is_paused](int row, int col) {
		if (is_paused) {
			int r = row - 1;
			int c = col - 1;
			if (r >= 0 && r < this->world.getRows() && c >= 0 && c <this->world.getCols()) {
			
				Particle *existing = this->world.at(r, c);
				// left click to place Particle
				if (existing == nullptr) {

					uint8_t red = 255, green = 255, blue = 255;
					bool stat = false;
					int life = 1000;
					float ix = 0.0f;
					float iy = 0.0f;

					switch (current_spawn_type) {
						case ParticleType::FIRE:
							red = 255;
							green = 0;
							blue = 0;
							stat = true;
							break;
						case ParticleType::WATER:
							red = 0;
							green = 0;
							blue = 255;
							break;
						case ParticleType::EARTH:
							red = 139;
							green = 69;
							blue = 19;
							stat = true;
							life = -1;
							break;
						case ParticleType::AIR:
							red = 200;
							green = 255;
							blue = 255;
							iy = -1.0f;					// Air floats up initially
							ix = (rand() % 3) - 1.0f; //Random slight drift left or right
							break;
						case ParticleType::DUST:
							red = 210;
							green = 180;
							blue = 140;
							break;
						case ParticleType::LIGHTNING:
							red = 255;
							green = 255;
							blue = 0;
							life = 15;
							iy = 1.0f;				// Shoots downward
							ix = (rand() % 3) - 1.0f; // Random slight scatter left or right
							break;
						case ParticleType::DIRT:
							red = 101;
							green = 67;
							blue = 33;
							break;
					}

					Particle p((float)r, (float)c, ix, iy, current_spawn_type, red, green, blue, stat, life);
					this->world.addParticle(p);
				} else {
					// Delete if clicking on existing
					existing->setLifetime(0);
				}
			}
		}	
	};
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
			target_fps++;
		} else if (input == 's' || input == 'S'){ //slow down frame rate
			target_fps--;
			if(target_fps < 1)
				target_fps = 1;
		} else if (input == 'l' || input == 'L'){ // load the world from disk
			world.load("world_save.txt");
		} else if (input == 'm' || input == 'M' || input == 'w' || input == 'W'){ // save to disk
			world.save("world_save.txt");
		} else if (input == 'c' || input == 'C') { // cycle Particle
			int next_type = (static_cast<int>(current_spawn_type) + 1) % 7;
			current_spawn_type = static_cast<ParticleType>(next_type);
		} else if (input == 'd' || input 'D'){
			draw_bridges();
		}

		if(!is_paused){
			if(world.alive_count() == 0) {
				is_paused = true;
			} else {
				world.physics();
			}
			frame++;
		}

		stringstream ss;
		ss << "\033[2J\033[1;1H";
		for(const auto &p : world.getParticles()){
			int r = (int)p.getRow();
			int c = (int)p.getCol();
			if (r >= 0 && r < world.getRows() && c >= 0 && c < world.getCol()) {
				ss << "\033["<< r + 1 << ";" << c + 1 << "H";
				ss << "\033[48;2;" <<  (int)p.getRed() << ";" << (int)p.getGreen() << ";" << (int)p.getBlue() << 'm';
				ss << "\033[0m";
			}
		}

		ss << "\033[" << world.getRows() + 2 << ";1H";
		ss << "\033[0m";
		ss << "Frame: " << frame << " | Alive: " << world.alive_count() << " | Target FPS: " << target_fps << " | Status: " <<(is_paused ? "PAUSED" : "RUNNING") << "          ";
		ss << "\033[" << world.getRows() + 3  << ";1H";
		ss << "Commands: (P)ause | (Q)uit | (C)ycle Element | (L)oad | Sa(v)e(M) | (F)aster | (Slower | (D)raw";

		cout << ss.str() << flush;

		auto end_time = chrono::steady_clock::now();
		auto elapsed = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
		int expected_delay = 1000000 / target_fps;
		if(elapsed < expected_delay){
			usleep(expected_delay - elapsed);
		}
	}
	
	set_raw_mode(false);
	set_cursor_mode(true);
	set_mouse_mode(false);
	resetcolor();
	clearscreen();
 }

void Game::render(){

}

void Game::draw_bridges(){
	bridges::Bridges bridges(1, "FGlez03", "1627533610787");
	bridges.setTitle("Particle Simulation");

	int r = world.getRows();
	int c = world.getCols();
	if (r <= 0 || c <= 0)
		return;

	bridges::ColorGrid cg(r, c, bridges::Color("black"));

	for(const auto &p : world.getParticles()){
		int pr = (int)p.getRow();
		int pc = (int)p.getCol();
		if (pr >= 0 && pr < r && pc >= 0 && pc < c) {
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
