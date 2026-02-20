#pragma once

#include "Particle.h"
#include <list>
#include <vector>
#include <string>
using namespace std;
class World {
	private:
	int rows;
	int cols;
	list<Particle> particles;
	vector<vector<ParticleType>> grid_map;
	
	public:
	World(int r, int c);
	Particle* at(int r, int c); //should return nullptr if empty
	int size();		//total particles
	int alive_count(); //particles with a lifetime of more than 0
	void physics();
	void save(string fileName);
	void load(string fileName);

};
