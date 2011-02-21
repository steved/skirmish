#include "terrain.h"

#include "SDL_gfxPrimitives.h"
#include <assert.h>

static float terrain[MAP_SIZE + 1][MAP_SIZE + 1];
static float diamond(float, float, float, float, float);
static float square(float, float, float);
static float cap(float);
static void blur();
static float random_float();
static float displace(float);

// random float from 0.0 to 1.0
static float random_float() {
  return (rand() / (float) RAND_MAX);
}

static float displace(float num) {
  float random = random_float() * num - (num / 2);
  return random;
}

void generate_fractal_terrain() {
  assert((MAP_SIZE & (MAP_SIZE - 1)) == 0);

  terrain[0][0] = random_float();
  terrain[MAP_SIZE][0] = random_float();
  terrain[0][MAP_SIZE] = random_float();
  terrain[MAP_SIZE][MAP_SIZE] = random_float();

  int side_length = MAP_SIZE >> 1;

  terrain[side_length][side_length] = diamond(terrain[0][0], terrain[MAP_SIZE][0],
      terrain[0][MAP_SIZE], terrain[MAP_SIZE][MAP_SIZE], 1);

  float displacement = 1 / NOISE;
  while(side_length) {
    int stride = MAP_SIZE / side_length;
    for(int i = 0; i < stride; i++) {
      for(int j = 0; j < stride; j++) {
        int xlen = i * side_length;
        int xllen = (i + 1) * side_length;
        int ylen = j * side_length;
        int yllen = (j + 1) * side_length;

        int midx = (xllen + xlen) / 2;
        int midy = (yllen + ylen) / 2;

        terrain[midx][midy] = diamond(terrain[xlen][ylen], terrain[xllen][ylen],
            terrain[xlen][yllen], terrain[xllen][yllen], displacement);

        terrain[midx][ylen] = square(terrain[xlen][ylen], terrain[xllen][ylen], displacement); // top left + top right
        terrain[midx][yllen] = square(terrain[xlen][ylen], terrain[xlen][yllen], displacement); // top left + bottom left
        terrain[xlen][midy] = square(terrain[xllen][ylen],terrain[xllen][yllen], displacement); // top right + bottom right
        terrain[xllen][midy] = square(terrain[xlen][yllen], terrain[xllen][yllen], displacement); // bottom left + bottom right
      }
    }
   
    side_length >>= 1;
    displacement /= NOISE;
  }

  blur();
  blur();
}

SDL_Surface *print_terrain() {
  int bpp = screen->format->BitsPerPixel;
  SDL_Surface *buffer = SDL_CreateRGBSurface(0, MAP_SIZE, MAP_SIZE, bpp, 0, 0, 0, 0);
  bpp = screen->format->BytesPerPixel;

  float min = 0, max = 0;
  for(int i = 0; i < MAP_SIZE; i++) {
    for(int j = 0; j < MAP_SIZE; j++) {
      float pixel = terrain[i][j];
      int c = 255 * pixel;

      if(pixel > max)
        max = pixel;
      if(pixel < min)
        min = pixel;

      uint8_t red = 0;
      uint8_t green = 0;
      uint8_t blue = 0;



      if(c < WATER) {
        blue = 92 + c;
      } else if (c >= WATER && c < FOREST) {
        green = (255 - c) / 1.3;
      } else if (c >= FOREST && c < MOUNTAINS) {
        red = blue = c / 1.2;
        green = c;
      } else {
        red = c / 1.4;
        green = (c + 80) / 1.5;
        blue = c / 1.6;
      }

      pixelRGBA(buffer, i, j, red, green, blue, 0xff);
    }
  }

  printf("terrain max %f, min %f\n", max, min);

  return buffer;
}

static void blur() {
  for(int i = 0; i < MAP_SIZE; i++) {
    for(int j = 0; j < MAP_SIZE; j++) {
      float accum = 0;
      int weights = 0;

      if(j > 0) {
        accum += terrain[i][j - 1];
        weights++;

        if(i > 0) {
          accum += terrain[i - 1][j - 1];
          weights++;
        }
        if(i < MAP_SIZE) {
          accum += terrain[i + 1][j - 1];
          weights++;
        }
      }
      if(i > 0) {
        accum += terrain[i - 1][j];
        weights++;
      }
      if(j < MAP_SIZE) {
        accum += terrain[i][j + 1];
        weights++;

        if(i > 0) {
          accum += terrain[i - 1][j + 1];
          weights++;
        }
        if(i < MAP_SIZE) {
          accum += terrain[i + 1][j + 1];
          weights++;
        }
      }
      if(i < MAP_SIZE) { 
        accum += terrain[i + 1][j];
        weights++;
      }

      terrain[i][j] = accum / weights;
    }
  }
}

static float diamond(float tleft, float tright, float bleft, float bright, float displacement) {
  float num = (tleft + tright + bleft + bright) / 4  + displace(displacement);
  return cap(num);
}

static float square(float left, float right, float displacement) {
  float num = (left + right) / 2 + displace(displacement);
  return cap(num);
}

static float cap(float num) {
  if(num > 1)
    num = 1;
  else if(num < 0)
    num = 0;
  return num;
}

float height_at(int x, int y) {
  return terrain[x][y];
}

float height_at_vector(gsl_vector *v) {
  return height_at(gsl_vector_get(v, 0), gsl_vector_get(v, 1));
}
