#include "collision.h"
#include "terrain.h"

bool bounding_circle_collision(gsl_vector *a, double radius_a, gsl_vector *b, double radius_b) {
  gsl_vector *temp = gsl_vector_calloc(3);
  gsl_vector_memcpy(temp, a);
  gsl_vector_sub(temp, b);
  gsl_vector_mul(temp, temp);

  double sum = gsl_vector_get(temp, 0) + gsl_vector_get(temp, 1); 
  gsl_vector_free(temp);

  double radius_sum_sq = radius_a + radius_b;
  radius_sum_sq *= radius_sum_sq;

  return radius_sum_sq > sum;
}

// is this vector allowed on this terrain
bool allowed_on_terrain(gsl_vector *vector) {
  float height = 255 * height_at(gsl_vector_get(vector, 0), gsl_vector_get(vector, 1));
  return height >= WATER; 
}
