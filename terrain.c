#include "terrain.h"

float terrain[MAP_SIZE + 1][MAP_SIZE + 1];

float random_float(float num) {
  float max = num / (float) (MAP_SIZE * 2) * HEIGHT_SCALE;
  float random = ((rand() / ((float) RAND_MAX)) - 0.5) * max;
  return random;
}

void generate_fractal_terrain() {
  assert((MAP_SIZE & (MAP_SIZE - 1)) == 0);

  terrain[0][0] = random_float(MAP_SIZE);
  terrain[MAP_SIZE][0] = random_float(MAP_SIZE);
  terrain[0][MAP_SIZE] = random_float(MAP_SIZE);
  terrain[MAP_SIZE][MAP_SIZE] = random_float(MAP_SIZE);

  int side_length = MAP_SIZE >> 1;

  terrain[side_length][side_length] = diamond(terrain[0][0], terrain[MAP_SIZE][0],
      terrain[0][MAP_SIZE], terrain[MAP_SIZE][MAP_SIZE], side_length);

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
            terrain[xlen][yllen], terrain[xllen][yllen], side_length);

        terrain[midx][ylen] = (terrain[xlen][ylen] + terrain[xllen][ylen]) / 2; // top left + top right
        terrain[midx][yllen] = (terrain[xlen][ylen] + terrain[xlen][yllen]) / 2; // top left + bottom left
        terrain[xlen][midy] = (terrain[xllen][ylen] + terrain[xllen][yllen]) / 2; // top right + bottom right
        terrain[xllen][midy] = (terrain[xlen][yllen] + terrain[xllen][yllen]) / 2; // bottom left + bottom right
      }
    }
   
    side_length >>= 1;
  }
}

SDL_Surface *print_terrain() {
  int bpp = screen->format->BitsPerPixel;
  SDL_Surface *buffer = SDL_CreateRGBSurface(0, MAP_SIZE, MAP_SIZE, bpp, 0, 0, 0, 0);
  bpp = screen->format->BytesPerPixel;

  for(int i = 0; i < MAP_SIZE; i++) {
    for(int j = 0; j < MAP_SIZE; j++) {
      float pixel = terrain[i][j];

      float red = 0;
      float green = 0;
      float blue = 0;

      if(pixel < 0.5) {
        red = pixel * 2;
      } else {
        red = (1 - pixel) * 2;
      }

      if(pixel >= 0.3 && pixel < 0.8) {
        green = (pixel - 0.3) * 2;
      } else if(pixel < 0.3) {
        green = (0.3 - pixel) * 2;
      } else {
        green = (1.3 - pixel) * 2;
      }

      if(pixel >= 0.5) {
        blue = (pixel - 0.5) * 2;
      } else {
        blue = (0.5 - pixel) * 2;
      }

      //printf("%f, %f, %f @ %d, %d\n", red, green, blue, i, j);
      pixelRGBA(buffer, i, j, 255 * red, 255 * green, 255 * blue, 0xff);
    }
  }

  return buffer;
}

float diamond(float tleft, float tright, float bleft, float bright, float displace) {
  return (tleft + tright + bleft + bright) / 4  + random_float(displace);
}
