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
		int old_r = (int)it->getRow();
		int old_c = (int)it->getCol();

		it->physics(*this);
	
		int new_r = (int)it->getRow();
		int new_c = (int)it->getCol();

		if (new_r < 0 || new_c < 0 || new_r >= rows || new_c >= cols || it->getLifetime() == 0) {
			if (old_r >= 0 && old_r < rows && old_c >= 0 && old_c < cols) {
				if (grid_ptrs[old_r][old_c] == &(*it)) {
					grid_ptrs[old_r][old_c] = nullptr;
					grid_map[old_r][old_c] = (char)-1;
				}
			}
			it = particles.erase(it);
		} else {
			if (old_r != new_r || old_c != new_c) {
				if (old_r >= 0 && old_r < rows && old_c >= 0 && old_c < cols) {
					if (grid_ptrs[old_r][old_c] == &(*it)) {
						grid_ptrs[old_r][old_c] = nullptr;
						grid_map[old_r][old_c] = (char)-1;
					}
				}
				grid_ptrs[new_r][new_c] = &(*it);
				grid_map[new_r][new_c] = static_cast<char>(it->getType());
			}
			++it;
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

