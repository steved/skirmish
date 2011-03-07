#include "collision.h"

#include "util/nav_mesh.h"
#include "util/terrain.h"

#include <gsl/gsl_blas.h>

bool bounding_circle_collision(gsl_vector *a, double radius_a, gsl_vector *b, double radius_b) {
  gsl_vector *temp = gsl_vector_calloc(3);
  gsl_vector_memcpy(temp, a);
  gsl_vector_sub(temp, b);
  gsl_vector_mul(temp, temp);

  // z vector seems to be causing trouble
  gsl_vector_set(temp, 2, 0); 

  double sum = gsl_blas_dasum(temp);
  gsl_vector_free(temp);

  double radius_sum_sq = radius_a + radius_b;
  radius_sum_sq *= radius_sum_sq;

  return radius_sum_sq >= sum;
}

// is this vector allowed on this terrain
bool allowed_on_terrain(gsl_vector *vector) {
  float height = height_at(gsl_vector_get(vector, 0), gsl_vector_get(vector, 1));
  return height >= WATER; 
}

// adapted Bresenham's line drawing algorithm
bool hit_water(int x, int y, int x2, int y2) {
  int dx = abs(x2 - x);
  int dy = abs(y2 - y);

  int sx = (x < x2) ? 1 : -1;
  int sy = (y < y2) ? 1 : -1;
  
  int err = dx - dy;
  int err2;

  while(1) {
    if(x == x2 && y == y2)
      break;

    if(height_at(x, y) < WATER + NODE_WATER_CUTOFF) {
      return true;
    }

    err2 = err << 1;
    if(err2 > -dy) {
      err -= dy;
      x += sx;
    }
    if(err2 < dx) {
      err += dx;
      y += sy;
    }
  }

  return false;
}


