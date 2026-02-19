#pragma once
#include <cstdint>
enum ParticleType {
		AIR,
		DUST,
		FIRE,
		WATER,
		EARTHH,
		DIRT,
		LIGHTNING

};

class Particle {

	private:
	float x_vel,y_vel; //velocity
	float col, row;    //position
	ParticleType type;

	struct Color {
	uint8_t r, g, b;
	} color;
	bool moving;
	int lifetime;   //kerney said use -1 for infinite

	public:

	Particle();
	Particle(float r, float c, float xv, float yv, ParticleType t, int life);
	float getRow() const;
	float getCol() const;
	ParticleType getType() const;

};
