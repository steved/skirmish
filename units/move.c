#include "units/move.h"

#include "util/terrain.h"

#include <gsl/gsl_blas.h>
#include <math.h>

//static void delta_height_scale(gsl_vector *, gsl_vector *);

bool move_unit_towards(unit *subj, gsl_vector *dest, PLAYERS *players) {
  // set the z coordinate because it gets set incorrectly when
  // placing the unit in the beginning 
  gsl_vector_set(subj->position, 2, height_at(x(subj->position), y(subj->position)));

  gsl_vector *go_to = gsl_vector_alloc(3);
  gsl_vector_memcpy(go_to, dest);
  gsl_vector_sub(go_to, subj->position);

  // check if we're already there
  bool there = abs(lround(x(go_to))) < subj->attributes.speed && abs(lround(y(go_to))) < subj->attributes.speed;
  if(there) {
    gsl_vector_free(go_to);
    return true;
  }

  double norm = gsl_blas_dnrm2(go_to);
  gsl_vector_scale(go_to, 1 / norm);

  //delta_height_scale(go_to, subj->position);
  gsl_vector_add(subj->velocity, go_to);

  //bool unit_at = check_for_unit_near(go_to, players, subj, false, false) != NULL;
  //if(!unit_at) {
    // XXX find a way around?
  //gsl_vector_memcpy(subj->position, go_to);
  //}

  // re-check if we're there yet
  gsl_vector_memcpy(go_to, dest);
  gsl_vector_sub(go_to, subj->position);
  there = abs(lround(x(go_to))) < subj->attributes.speed && abs(lround(y(go_to))) < subj->attributes.speed;

  gsl_vector_free(go_to);

  return there;
}

/*
static void delta_height_scale(gsl_vector *new_location, gsl_vector *location) {
  gsl_vector *delta_height = gsl_vector_alloc(3);
  gsl_vector_memcpy(delta_height, new_location);
  gsl_vector_add(delta_height, location);

  double delta = height_at_vector(delta_height) - gsl_vector_get(location, 2);
  gsl_vector_set(new_location, 2, delta);

  float sign; 
  delta *= 100;
  if(delta < -0.1)
    sign = 1;
  else if(delta > 0.1)
    sign = -1;
  else
    sign = 0;

  // TODO scale by how high delta height is
  gsl_vector_scale(new_location, 1 + (sign * 0.3)); 
  gsl_vector_set(new_location, 2, height_at_vector(delta_height) - gsl_vector_get(location, 2));
}*/
