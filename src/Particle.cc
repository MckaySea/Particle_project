#include "../include/Particle.h"
#include "../include/World.h"
#include <cstdlib>
using namespace std;

Particle::Particle() {
	row = 0;
	col = 0;
	x_vel = 0;
	y_vel = 0;
	type = ParticleType::AIR;
	stationary = false;
	lifetime = 0;
	color.Red = 255;
	color.Blue = 255;
}

Particle::Particle(float r, float c, float xv, float yv, ParticleType t, uint8_t red, uint8_t green, uint8_t blue, bool stat, int life) {
	row = r;
	col = c;
	x_vel = xv;
	y_vel = yv;
	type = t;
	stationary = stat;
	lifetime = life;
	color.Red = red;
	color.Green = green;
	color.Blue = blue;
}

int Particle::getLifetime() const {
	return lifetime;
}
void Particle::setLifetime(int life) {
	lifetime = life;
}
float Particle::getRow() const {
	return row;
}
void Particle::setRow(float r) {
	row = r;
}
float Particle::getCol() const {
	return col;
}
void Particle::setCol(float c) {
	col = c;
}
float Particle::getX_Vel() const {
	return x_vel;
}
void Particle::setX_Vel(float xv) {
	x_vel = xv;
}
float Particle::getY_Vel() const {
	return y_vel;
}
void Particle::setY_Vel(float yv) {
	y_vel = yv;
}
ParticleType Particle::getType() const {
	return type;
}
void Particle::setParticleType(ParticleType t) {
	type = t;
}
bool Particle::get_stationary() const {
	return stationary;
}
void Particle::set_stationary(bool stat) {
	stationary = stat;
}
uint8_t Particle::getRed() const {
	return color.Red;
}
void Particle::setRed(uint8_t red) {
	color.Red = red;
}
uint8_t Particle::getGreen() const {
	return color.Green;
}
void Particle::setGreen(uint8_t green) {
	color.Green = green;
}
uint8_t Particle::getBlue() const {
	return color.Blue;
}
void Particle::setBlue(uint8_t blue) {
	color.Blue = blue;
}

void Particle::touch(Particle &other) {

	if (this->type == ParticleType::WATER && other.getType() == ParticleType::FIRE) {
		this->type = ParticleType::AIR;
		this->y_vel = -1.0f;
		this->x_vel = 0.0f;
		this->color.Red = 200;
		this->color.Green = 255;
		this->color.Blue = 255;
		this->lifetime = 100;
		this->stationary = false;
	}
	else if (this->type == ParticleType::FIRE && other.getType() == ParticleType::WATER) {
		other.setParticleType(ParticleType::AIR);
		other.setX_Vel(0.0f);
		other.setY_Vel(-1.0f);
		other.setRed(200);
		other.setGreen(255);
		other.setBlue(255);
		other.setLifetime(100);
		other.set_stationary(false);
	}

	if (this->type == ParticleType::LIGHTNING && other.getType() == ParticleType::WATER) {
		other.setParticleType(ParticleType::LIGHTNING);
		other.setRed(255);
		other.setGreen(255);
		other.setBlue(0);
		other.setLifetime(15);
		other.setX_Vel(this->x_vel);
		other.setY_Vel(this->y_vel);
	}
	else if (this->type == ParticleType::WATER && other.getType() == ParticleType::LIGHTNING) {
		this->type = ParticleType::LIGHTNING;
		this->color.Red = 255;
		this->color.Green = 255;
		this->color.Blue = 0;
		this->lifetime = 15;
		this->x_vel = other.getX_Vel();
		this->y_vel = other.getY_Vel();
	}

}

void Particle::physics(World& world) {
		if (lifetime > 0) lifetime--;
		if (!stationary) return;
		switch(type) {

			case ParticleType::DUST:

			break;
				//code for dust effect on each particle 
				//Dust has a small amount of gravity and randomly moves left and right every frame
			case ParticleType::AIR:

			break;
				//code to implement that affects each particle for water
				//Air moves in a straight line (ignoring gravity) bouncing off solid
			break;

			case ParticleType::FIRE:
				//implement code here for air effect
				//Fire is stationary and shoots sparks of lightning in different directions over time
			break;

			case ParticleType::WATER:
				//implement code for for dirst effect
				//Water drips down and if it hits something solid it will slide sideways to find the lowest level. Water touching fire turns into
				//air moving upwards
			break;

			case ParticleType::EARTH:
				//implement code for earth effect
				//Earth is always stationary and solid
			break;

			case ParticleType::DIRT:
				//implement code for the dirt effect
				//Dirt travels downwards and forms piles when it hits something solid
			break;

			case ParticleType::LIGHTNING:
				//implement code for the lightning effect
				//Dirt travels downwards and froms piles when it hits something
			break;

			default:
			break;
		}
}
