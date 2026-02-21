#pragma once
#include  <cstdint>
enum class ParticleType {
		AIR,
		DUST,
		FIRE,
		WATER,
		EARTH,
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
	uint8_t Red, Green, Blue;
	} color;
	bool stationary;
	int lifetime;   //kerney said use -1 for infinite
	int gravity;

	public:

	Particle();
	Particle(float r, float c, float xv, float yv, ParticleType t,uint8_t red, uint8_t green, uint8_t blue, bool stat, int life);
	float getRow() const;
	float getCol() const;
	float getX_Vel() const;
	float getY_Vel() const;
	bool get_stationary() const;
	uint8_t getRed() const;
	uint8_t getGreen() const;
	uint8_t getBlue() const; 
	ParticleType getType() const;
	int getLifetime() const;
	

	//core logic stuff, i forward declared world to pass it as a reference so we can cehck surrondings
	void physics(World& world);
	void touch(Particle& other);

};
