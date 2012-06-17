#ifndef LIBFULLCIRCLE_PERLIN_NOISE_HPP
#define LIBFULLCIRCLE_PERLIN_NOISE_HPP 1

// Derived from http://www.flipcode.com/archives/Perlin_Noise_Class.shtml

#include <libfullcircle/common.hpp>

namespace fullcircle {

  const uint16_t SAMPLE_SIZE=1024;

  class PerlinNoise {
    public:
      typedef std::tr1::shared_ptr<PerlinNoise> Ptr;
      PerlinNoise (int octaves,float freq,float amp,int seed);
      float get_2d(float x, float y);
      float get_3d(float x, float y, float z);
      virtual ~PerlinNoise() {};

    private:
      PerlinNoise (const PerlinNoise& original);
      PerlinNoise& operator= (const PerlinNoise& rhs);
      void init_perlin(int n,float p);
      float perlin_noise_2D(float vec[2]);
      float perlin_noise_3D(float vec[3]);

      float noise1(float arg);
      float noise2(float vec[2]);
      float noise3(float vec[3]);
      void normalize2(float v[2]);
      void normalize3(float v[3]);
      void init(void);

      int   _octaves;
      float _frequency;
      float _amplitude;
      int   _seed;
      bool  _start;

      int p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
      float g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];
      float g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
      float g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];


  };
};


#endif /* LIBFULLCIRCLE_PERLIN_NOISE_HPP */

