#include "../include/World.h"
#include "../include/Particle.h"
#include <iostream>
#include <fstream>
using namespace std;

World::World(int r, int c) {
	rows = r;
	cols = c;
	grid_map.assign(r, vector<char>(c, (char)-1));
	grid_ptrs.assign(r, vector<Particle *>(c, nullptr));
}

Particle *World::at(int r, int c) {
	if (r < 0 || r >= rows || c < 0 || c >= cols)
		return nullptr;
	return grid_ptrs[r][c];
}

int World::size() {
		return particles.size();
}

int World::alive_count() {
	int count = 0;

	for (const auto& p : particles) {
		if (p.getLifetime() > 0 || p.getLifetime() == -1) {
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
		p.setXVel(vx);
		p.setYVel(vy);
		p.setRGB(r_val, g_val, b_val);
		p.setStationary(stat_val);
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
			<< static_cast<int>(p.getR()) << " "
			<< static_cast<int>(p.getG()) << " "
			<< static_cast<int>(p.getB()) << " "
			<< p.getStationary() << " "
			<< p.getLifetime() << "\n";
	}
	out.close();
}

