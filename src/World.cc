#include "../include/World.h"
#include "../include/Particle.h"
#include <fstream>
#include <iostream>

using namespace std;

World::World(int r, int c) {
  rows = r;
  cols = c;
  // Initialize with -1 represebtubg empty spacce 
  grid_map.assign(r, vector<char>(c, (char)-1));
  // Initialize the pointer grid (grid_ptrs) with nullptr
  grid_ptrs.assign(r, vector<Particle *>(c, nullptr));
}

//returns a pointer to the particle at a specific row n col
Particle *World::at(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= cols)
    return nullptr;
  return grid_ptrs[r][c];
}

int World::size() { return particles.size(); }

int World::alive_count() {
  int count = 0;
  for (const auto &p : particles) {
    if (p.getLifetime() > 0 || p.getLifetime() == -1) {
      count++;
    }
  }
  return count;
}

void World::addParticle(const Particle &p) {
  int r = (int)p.getRow();
  int c = (int)p.getCol();
  // check to see spawn is in bounds
  if (r >= 0 && r < rows && c >= 0 && c < cols) {

    // If the spot is already occupied by another particle we di bithubg
    if (grid_ptrs[r][c] != nullptr)
      return; 
    particles.push_back(p);

    // Poins the grid at the particile we just added, at the back of the vec
    grid_ptrs[r][c] = &particles.back();

    //update map visualy w the particles type
    grid_map[r][c] = static_cast<char>(p.getType());
  }
}

void World::physics() {
  //iteratin throughe every particle in tghe list for safe deletion
  for (auto it = particles.begin(); it != particles.end();) {
    int old_r = (int)it->getRow();
    int old_c = (int)it->getCol();

    it->physics(*this);

    int new_r = (int)it->getRow();
    int new_c = (int)it->getCol();
  //check if particle should b removed because its ound of bounds or end of life
    if (new_r < 0 || new_c < 0 || new_r >= rows || new_c >= cols ||
        it->getLifetime() == 0) {
      // clears the grid entry where particle use to be
      if (old_r >= 0 && old_r < rows && old_c >= 0 && old_c < cols) {
        if (grid_ptrs[old_r][old_c] == &(*it)) {
          grid_ptrs[old_r][old_c] = nullptr;
          grid_map[old_r][old_c] = (char)-1;
        }
      }
      //remove the particle and moves 2 the next valid element
      it = particles.erase(it);
    } else {
      // If the particle moved to a new cell within bounds
      if (old_r != new_r || old_c != new_c) {
        //clear the old grid cell
        if (old_r >= 0 && old_r < rows && old_c >= 0 && old_c < cols) {
          if (grid_ptrs[old_r][old_c] == &(*it)) {
            grid_ptrs[old_r][old_c] = nullptr;
            grid_map[old_r][old_c] = (char)-1;
          }
        }
        // updats the new grid cell with the current particles ptr/type
        grid_ptrs[new_r][new_c] = &(*it);
        grid_map[new_r][new_c] = static_cast<char>(it->getType());
      }
      ++it;
    }
  }
}

void World::load(string fileName) {
  ifstream in(fileName);
  if (!in.is_open())
    return;

  particles.clear();

  int saved_rows, saved_cols;
  in >> saved_rows >> saved_cols;
  rows = saved_rows;
  cols = saved_cols;
  grid_map.assign(rows, vector<char>(cols, (char)-1));
  grid_ptrs.assign(rows, vector<Particle *>(cols, nullptr));

  int type_val, r_val, g_val, b_val, life;
  bool stat_val;
  float pr, pc, vx, vy;

  while (in >> type_val >> pr >> pc >> vx >> vy >> r_val >> g_val >> b_val >>
         stat_val >> life) {
    Particle p(pr, pc, vx, vy, static_cast<ParticleType>(type_val), r_val,
               g_val, b_val, stat_val, life);
    addParticle(p);
  }
  in.close();
}

void World::save(string fileName) {
  ofstream out(fileName);
  if (!out.is_open())
    return;

  out << rows << " " << cols << "\n";

  for (const auto &p : particles) {
    out << static_cast<int>(p.getType()) << " " << p.getRow() << " "
        << p.getCol() << " " << p.getX_Vel() << " " << p.getY_Vel() << " "
        << static_cast<int>(p.getRed()) << " " << static_cast<int>(p.getGreen())
        << " " << static_cast<int>(p.getBlue()) << " " << p.get_stationary()
        << " " << p.getLifetime() << "\n";
  }
  out.close();
}
