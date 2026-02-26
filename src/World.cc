#include "../include/World.h"
#include "../include/Particle.h"
#include <iostream>
#include <list>
#include <fstream>
using namespace std;

World::World(int r, int c) {
	rows = r;
	cols = c;

	for (auto it = particles.begin(); it != particles.end();) {
		if (it->getRow() < 0 || it->getCol() < 0 || it->getRow() >= rows || it->getCol() >= cols) {
			particles.erase(it); //any point which leaves boundary gets deleted
		 }
	}
}

int World::size() {
		return particles.size();
}

int World::alive_count() {
	int count = 0;

	for (const auto& p : particles) {

		if (p.getLifetime() > 0) {
			count++;
		}
	}
	
	return count;
}

void World::physics() {
	for (auto it = particles.begin(); it != particles.end();) {
		it->physics(*this);
	
		if (it->getRow() < 0 || it->getCol() < 0 || it->getRow() >= rows || it->getCol() >= cols || it->getLifetime() == 0) {
			particles.erase(it); //if particle moves out of bounds, delete
		}
	
		else {
			for (const auto& p : particles) {
				cout << p << " "; }
		}
	}
}

void World::load() { //load world from disk
	ifstream in;
	in.open("world_save.txt");
	if (!in.is_open()) return;

	particles.clear();
	for (auto& row_vec : map) {
		std::fill(row_vec.begin(), row_vec.end(), 0);
	}

	int saved_rows, saved_cols;
	in >> saved_rows >> saved_cols;

	int type_val, r_val, g_val, b_val, stat_val, life;
	float pr, pc, vx, vy;

	while (in >> type_val >> pr >> pc >> vx >> vy >> r_val >> g_val >> b_val >> stat_val >> life) {
		Particle p(static_cast<ParticleType>(type_val), pr, pc);
		p.setX_Vel(vx);
		p.setY_Vel(vy);
		p.setRGB(r_val, g_val, b_val);
		p.set_stationary(stat_val);
		p.setLifetime(life);

		particles.push_back(p);
	}
	in.close();
}

void World::save() { //save world to disk
	ofstream out;
	out.open("world_save.txt");
	if (!out.is_open()) return;

	out << rows << " " << cols << "\n";

	for (const auto& p : particles) {
		out << static_cast<int>(p.getType()) << " " 
			<< p.getRow() << " " << p.getCol() << " "
			<< static_cast<int>(p.getRed()) << " "
			<< static_cast<int>(p.getGreen()) << " "
			<< static_cast<int>(p.getBlue()) << " "
			<< p.get_stationary() << " "
			<< p.getLifetime() << "\n";
	}
	out.close();
}

