#include "/public/colors.h"
#include "../include/Particle.h"
#include "../include/World.h"
using namespace std;

Particle::Particle(float r, float c, float xv, float yv, ParticleType t, uint8_t Red, uint8_t Green, uint8_t Blue, bool stat, int life) {
	row = r;
	col = c;
	x_vel = xv;
	y_vel = yv;
	type = t;
	stationary = stat;
	lifetime = life;
	switch(type) {
		case ParticleType::FIRE:
			Red = 255; Green = 0; Blue = 0;
			break;
		case ParticleType::WATER:
			Red = 0; Green = 100; Blue = 255;
			break;
		case ParticleType::EARTH:
			Red = 139; Green = 69; Blue = 19;
			break;
		case ParticleType::AIR:
			Red = 200; Green = 255; Blue = 255;
			break;
		case ParticleType::DUST:
			Red = 210; Green = 180; Blue = 140;
			break;
		case ParticleType::LIGHTNING:
			Red = 255; Green = 255; Blue = 0;
			break;
		case ParticleType::DIRT:
			Red = 101; Green = 67; Blue = 33;
			break;
		default:
			Red = 255; Green = 255; Blue = 255;
	}
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
	Red = red;
}
uint8_t Particle::getGreen() const {
	return color.Green;
}
void Particle::setGreen(uint8_t green) {
	Green = green;
}
uint8_t Particle::getBlue() const {
	return color.Blue;
}
void Particle::setBlue(uint8_t blue) {
	Blue = blue;
}
void Particle::physics(World& world) {
		if (lifetime > 0) lifetime--;
		if (!stationary) return;
		switch(type) {

			case ParticleType::DUST:

				//going str8 downwards
				/*int smallgrav = 2.45;
				if(world.at(row + 1, col) == nullptr) {
					row += smallgrav;
				} else if(world.at(row + 1, col - 1) == nullptr) { //down left if straight down is blocked
					row++;
					col--;
				} else if(world.at(row + 1, col + 1) == nullptr) { //down right if down left and str8 down are blocked
					row++;
					col++;
				} else if(world.at(row + 1, col) == nullptr) {
					world.at(row + 1, col)->touch(*this);
				}
				*/
				break;
				//code for dust effect on each particle 
				//Dust has a small amount of gravity and randomly moves left and right every frame
			case ParticleType::AIR:
				if(world.at(row - 1, col) == nullptr) {
					row--;
				} else if(world.at(row - 1, col - 1) == nullptr) { //up left if straight up is blocked
					row--;
					col--;
				} else if(world.at(row - 1, col + 1) == nullptr) { //up right if up left and str8 up are blocked
					row--;
					col++;
				} else if(world.at(row - 1, col) == nullptr) {
					world.at(row - 1, col)->touch(*this);
				}
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

void Particle::touch(Particle& other) {

}
