#include "terrain.h"
#include "unit_state_functions.h"

#include <stdbool.h>
#include <gsl/gsl_blas.h>

static bool close(gsl_vector *, gsl_vector *);
void (*state_functions[5])(unit *) = { &do_wait, &do_attack, &do_charge, &do_retreat, &do_move };

void do_wait(unit *u) {}
void do_attack(unit *u) {}
void do_charge(unit *u) {}
void do_retreat(unit *u) {}
void do_move(unit *u) {
  // set the z coordinate because it gets set incorrectly when
  // placing the unit in the beginning 
  gsl_vector_set(u->vector, 2, height_at(gsl_vector_get(u->vector, 0), gsl_vector_get(u->vector, 1)));

  gsl_vector *go_to = gsl_vector_alloc(3);
  gsl_vector_memcpy(go_to, u->state.subject.vector);
  gsl_vector_sub(go_to, u->vector);

  double norm = gsl_blas_dnrm2(go_to);
  gsl_vector_scale(go_to, 1 / norm);

  double delta_height = height_at(gsl_vector_get(go_to, 0) + gsl_vector_get(u->vector, 0),
      gsl_vector_get(go_to, 1) + gsl_vector_get(u->vector, 1)) - gsl_vector_get(u->vector, 2);
  gsl_vector_set(go_to, 2, delta_height);

  float sign; 
  delta_height *= 100;
  if(delta_height < -0.1)
    sign = 1;
  else if(delta_height > 0.1)
    sign = -1;
  else
    sign = 0;

  // TODO scale by how high delta height is
  gsl_vector_scale(go_to, 1 + (sign * 0.3)); 
  delta_height = height_at(gsl_vector_get(go_to, 0) + gsl_vector_get(u->vector, 0),
      gsl_vector_get(go_to, 1) + gsl_vector_get(u->vector, 1)) - gsl_vector_get(u->vector, 2);
  gsl_vector_set(go_to, 2, delta_height);

  gsl_vector_add(u->vector, go_to);
  gsl_vector_free(go_to);

  if(close(u->vector, u->state.subject.vector)) {
    u->state.current = waiting;
    u->state.subject.vector = NULL;
    printf("switching to wait state\n");
  }
}

bool close(gsl_vector *subj, gsl_vector *dest) {
  gsl_vector *temp = gsl_vector_alloc(3);
  gsl_vector_memcpy(temp, subj);
  gsl_vector_sub(temp, dest);
  gsl_vector_set(temp, 2, 0);
  double max, min;
  gsl_vector_minmax(temp, &min, &max);
  gsl_vector_free(temp);

  return abs(max) <= 3 && abs(min) <= 3;
}
