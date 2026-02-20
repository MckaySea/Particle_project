#include "/public/colors.h"
#include "../include/Particle.h"
#include "../include/World.h"
using namespace std;

Particle::Particle(float r, float c, float xv, float yv, ParticleType t, int life) {
	row = r;
	col = c;
	x_vel = xv;
	y_vel = yv;
	type = t;
	lifetime = life;
}
float Particle::getRow() const {
	return row;
}
float Particle::getCol() const {
	return col;
}
ParticleType Particle::getType() const {
	return type;
}
void Particle::physics(World& world) {
		if (lifetime > 0) lifetime--;
		if (!moving) return;
		switch(type) {

			case ParticleType::AIR:
				//code for dust effect on each particle 
				//Air moves in a straight line (ignoring gravity) bouncing off solid
			break;

			case ParticleType::DUST:
				//code to implement that affects each particle for water
				//Dust has a small amount of gravity and randomly moves left and right every frame
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
