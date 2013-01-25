#ifndef LIBFULLCIRCLE_SFX_GAME_OF_LIFE_HPP
#define LIBFULLCIRCLE_SFX_GAME_OF_LIFE_HPP 1

#include <libfullcircle/common.hpp>
#include <boost/multi_array.hpp>

namespace fullcircle {

  typedef uint8_t cell_t;
  typedef boost::multi_array<cell_t, 2> generation_t;
  const cell_t DEAD = 0;
  const cell_t ALIVE = 1;

  class GameOfLife {
    public:
      typedef boost::shared_ptr<GameOfLife> Ptr;
      GameOfLife (
          const uint16_t& width, 
          const uint16_t& height,
          const uint16_t& seed,
          const uint16_t& density);
      uint16_t get_width() const { return _width;};
      uint16_t get_height() const { return _height;};
      const generation_t get_next_generation();
      virtual ~GameOfLife() {};

    private:
      GameOfLife (const GameOfLife& original);
      GameOfLife& operator= (const GameOfLife& rhs);
      void initialize_from_seed(const uint16_t& seed, const uint16_t& density);
      void simple_initialize();
      void swap_generations();
      void fill_generation(generation_t& gen, const cell_t& value);
      const uint8_t count_neighbor_cells(
          const generation_t& gen,
          const uint16_t& x, 
          const uint16_t& y
        );
      const uint16_t _width;
      const uint16_t _height;
      generation_t _parents; // The current generation
      generation_t _children; // The next generation

  };
};


#endif /* LIBFULLCIRCLE_SFX_GAME_OF_LIFE_HPP */

