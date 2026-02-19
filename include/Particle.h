#pragma once
#include  <cstdint>
enum class ParticleType {
		AIR,
		DUST,
		FIRE,
		WATER,
		EARTHH,
		DIRT,
		LIGHTNING

};
class World;
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
	

	//core logic stuff, i forward declared world to pass it as a reference so we can cehck surrondings
	void physics(World& world);
	void touch(Particle& other);

};
