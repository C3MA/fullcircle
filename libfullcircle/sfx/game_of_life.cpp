#include "game_of_life.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


using namespace fullcircle;

GameOfLife::GameOfLife(
          const uint16_t& width, 
          const uint16_t& height,
          const uint16_t& seed,
          const uint16_t& density
) : _width(width)
  , _height(height)
  , _parents(boost::extents[_width][_height])
  , _children(boost::extents[_width][_height])
{
  fill_generation(_parents, DEAD);
  fill_generation(_children, DEAD);
  initialize_from_seed(seed, density);
//  simple_initialize();
}

/**
 * For debugging purposes.
 */
void GameOfLife::simple_initialize() {
  _parents[1][0]=ALIVE;
}

void GameOfLife::initialize_from_seed(const uint16_t& seed, const uint16_t& density) {
  boost::random::mt19937 gen(seed);
  boost::random::uniform_int_distribution<> x_dist(0, get_width()-1);
  boost::random::uniform_int_distribution<> y_dist(0, get_height()-1);
  for( uint16_t i = 0; i < density; ++i) {
    _parents[x_dist(gen)][y_dist(gen)] = ALIVE;
  }
}

void GameOfLife::swap_generations() {
  for( uint16_t x = 0; x < get_width(); ++x) {
    for( uint16_t y = 0; y < get_height(); ++y) {
      _parents[x][y]=_children[x][y];
    }
  }
  fill_generation(_children, DEAD);
}

void GameOfLife::fill_generation(generation_t& gen, const cell_t& value) {
  for( uint16_t x = 0; x < get_width(); x++) {
    for( uint16_t y = 0; y < get_height(); y++) {
      gen[x][y]=value;
    }
  }
}

/**
 * Count all neighbors around a given cell.
 */
const uint8_t GameOfLife::count_neighbor_cells(
    const generation_t& gen,
    const uint16_t& x, 
    const uint16_t& y
) {
  uint8_t count=0;
  int left_idx=x-1;
  int right_idx=x+1;
  int up_idx=y-1;
  int down_idx=y+1;
  // Check for border cells - we just wrap around the field.
  if (left_idx < 0) left_idx=get_width()-1;
  if (right_idx == get_width()) right_idx=0;
  if (up_idx < 0) up_idx=get_height()-1;
  if (down_idx == get_height()) down_idx=0;
    // Check all neighbors
  count += gen[left_idx][up_idx];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[x][up_idx];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[right_idx][up_idx];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[left_idx][y];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[right_idx][y];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[left_idx][down_idx];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[x][down_idx];
   // std::cout << "- count: " << (int)count << std::endl;
  count += gen[right_idx][down_idx];
  //  std::cout << "- count: " << (int)count << std::endl;
  //if (x == 0 && y == 0) {
  //  std::cout << "Width: " << get_width() << std::endl;
  //  std::cout << "Height: " << get_height() << std::endl;
  //  std::cout << "left: " << left_idx << std::endl;
  //  std::cout << "right: " << right_idx << std::endl;
  //  std::cout << "up: " << up_idx << std::endl;
  //  std::cout << "down: " << down_idx << std::endl;
  //  std::cout << "count: " << (int)count << std::endl;
  //  exit(2);
  //}

  return count;
}

const generation_t GameOfLife::get_next_generation() {
  for( uint16_t x = 0; x < get_width(); ++x) {
    for( uint16_t y = 0; y < get_height(); ++y) {
      //std::cout << "Neighborhood of " << x << ":" << y << " is " << (int)count_neighbor_cells(_parents,x,y) << std::endl;
      // Check rules.
      int neighbors=count_neighbor_cells(_parents, x, y);
      switch (neighbors) {
        // 1. Loneliness
        case 0: _children[x][y]=DEAD; break;
        case 1: _children[x][y]=DEAD; break;
        // 2. Two or three neighbors are fine
        case 2: _children[x][y]=ALIVE; break;
        case 3: _children[x][y]=ALIVE; break;
        // overpopulation
        case 4: _children[x][y]=DEAD; break;
        case 5: _children[x][y]=DEAD; break;
        case 6: _children[x][y]=DEAD; break;
        case 7: _children[x][y]=DEAD; break;
        case 8: _children[x][y]=DEAD; break;
        default:
                std::cout << 
                  "Problem during generation update - "
                  << neighbors << " neighbors." << std::endl;
                break;
      }
    }
  }
  swap_generations();
  return _parents;
}
