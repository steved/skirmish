#include "util/random.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

gsl_rng *rand_gen = NULL;

float random_float() {
  return (float) gsl_rng_uniform(rand_gen);
}

int random_int_max(int max) {
  return gsl_rng_uniform_int(rand_gen, max);
}

int random_int() {
  return gsl_rng_get(rand_gen);
}

void allocate_rng() {
  free_rng();
  rand_gen = gsl_rng_alloc(gsl_rng_ranlux);
  gsl_rng_set(rand_gen, time(NULL));
}

void free_rng() {
  if(rand_gen != NULL)
    gsl_rng_free(rand_gen);
}
