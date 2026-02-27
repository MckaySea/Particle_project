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

	if (this->type == ParticleType::LIGHTNING && other.getType() == ParticleType::EARTH) {
		other.setParticleType(ParticleType::DIRT);
		other.setRed(101);
		other.setGreen(67);
		other.setBlue(33);
		other.set_stationary(false);
		other.setY_Vel(1.0f);
		other.setLifetime(1000);
	}
	else if (this->type == ParticleType::EARTH && other.getType() == ParticleType::LIGHTNING) {
		this->type = ParticleType::DIRT;
		this->color.Red = 101;
		this->color.Green = 67;
		this->color.Blue = 33;
		this->stationary = false;
		this->y_vel = 1.0f;
		this->lifetime = 1000;
	}
}

void Particle::physics(World& world) {

		if (lifetime > 0) lifetime--;

		if (type == ParticleType::FIRE) {
			if (rand() % 20 == 0 ) {
				float lx = (rand() % 3) - 1.0f;
				float ly = (rand() % 3) - 1.0f;
				if (lx != 0.0f || ly != 0.0f) {
					int targetRow = (int)(row + ly);
					int targetCol = (int)(col + lx);
					if (targetRow >= 0 && targetRow < world.getRows() && targetCol >= 0 && targetCol < world.getCols()) {
						if (world.at(targetRow, targetCol) == nullptr) {
							Particle p(targetRow, targetCol, lx * 2.0f, ly * 2.0f, ParticleType::LIGHTNING, 255, 255, 0, false, 15);
							world.addParticle(p);
						}
					}
				}
			}
		}

		if (stationary) return;
		
		float next_row = row + y_vel;
		float next_col = col + x_vel;
		Particle *hit = world.at((int)next_row, (int)next_col);

		if (hit != nullptr && hit != this) {
			this->touch(*hit);
		}


		switch(type) {

			case ParticleType::AIR:
				next_row = row + y_vel;
				next_col = col +x_vel;
				hit = world.at((int)next_row, (int)next_col);
				if (hit == this) hit == nullptr;
				if (hit && hit->get_stationary()) {
					x_vel = -x_vel;
					y_vel = -y_vel;
					row += y_vel;
					col += x_vel;
				}
				else if (!hit) {
					row = next_row;
					col = next_col;
				}
				else {
					x_vel = -x_vel;
					y_vel = -y_vel;
					row += y_vel;
					col += x_vel;
				}
				break;

			case ParticleType::DUST:
				y_vel += 0.2f;
				if (y_vel > 1.0f) y_vel = 1.0f;
				x_vel = (rand() % 3) - 1;
				next_row = row + y_vel;
				next_col = col + x_vel;
				hit = world.at((int)next_row, (int)next_col);
				if (hit == this) hit == nullptr;
				if (next_row >= world.getRows()) {
					y_vel = 0;
					row = world.getRows() - 1;
				}
				else if (!hit) {
					row = next_row;
					col = next_col;
				}
				else {
					hit = world.at((int)next_row, (int)col);
					if (hit == this) hit = nullptr;
					if (!hit) row = next_row;
					else y_vel = 0;
				}
				break;

			case ParticleType::WATER:
				y_vel += 0.5f;
				if (y_vel > 1.0f) y_vel = 1.0f;
				next_row = row + y_vel;
				hit = world.at((int)next_row, (int)col);
				if (hit == this) hit = nullptr;
				if (next_row >= world.getRows()) {
					next_row = world.getRows() - 1;
					hit = this;
				}
				if (!hit) {
					row = next_row;
				}
				else {
					y_vel = 0;
					bool left_down = ((int)row + 1 >= world.getRows())
						? false
						: (world.at((int)row + 1, (int)col - 1) != nullptr && world.at((int)row + 1, (int)col - 1) != this
								? false
								: true);
					bool right_down = ((int)row + 1 >= world.getRows()) 
						? false
						: (world.at((int)row + 1, (int)col + 1) != nullptr && world.at((int)row + 1, (int)col + 1) != this
								? false
								: true);
					bool left = world.at((int)row, (int)col - 1) != nullptr && world.at((int)row, (int)col - 1) != this
						? false
						: true;
					bool right = world.at((int)row, (int)col + 1) != nullptr && world.at((int)row, (int)col + 1) != this
						? false
						: true;

					if (left && left_down && right && right_down) {
						col += (rand() % 2 == 0) ? -1 : 1;
					}
					else if (left && left_down) {
						col -= 1;
					}
					else if (right && right_down) {
						col += 1;
					}
					else if (left && right) {
						col += (rand() % 2 == 0) ? -1 : 1;
					}
					else if (left) {
						col -= 1;
					}
					else if (right) {
						col += 1;
					}
					else {
						//Can't slide left or right downwards
					}
				}
				break;

			case ParticleType::DIRT:
				//implement code for earth effect
				//Earth is always stationary and solid
			break;

			case ParticleType::LIGHTNING:
				//implement code for the dirt effect
				//Dirt travels downwards and forms piles when it hits something solid
			break;

			case ParticleType::EARTH:
				//implement code for the lightning effect
				//Dirt travels downwards and froms piles when it hits something
			break;

			default:
			break;
		}
}
