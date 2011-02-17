#include "collision.h"
#include "player.h"
#include "terrain.h"
#include "units.h"
#include "unit_state_functions.h"

#include <stdbool.h>
#include <gsl/gsl_blas.h>

void (*state_functions[5])(PLAYERS *, camera *, unit *) = { &do_wait, &do_attack, &do_charge, &do_retreat, &do_move };

void do_wait(PLAYERS *players, camera *camera, unit *u) {}
void do_attack(PLAYERS *players, camera *camera, unit *u) {}
void do_charge(PLAYERS *players, camera *camera, unit *u) {}
void do_retreat(PLAYERS *players, camera *camera, unit *u) {}
void do_move(PLAYERS *players, camera *camera, unit *u) {
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

  gsl_vector_add(go_to, u->vector);

  bool unit_at = check_for_unit_near(go_to, camera, players, u);
  if(allowed_on_terrain(go_to) && !unit_at) {
    // find a way around?
    gsl_vector_memcpy(u->vector, go_to);
  }

  gsl_vector_free(go_to);

  if(bounding_circle_collision(u->vector, unit_radius[u->type], u->state.subject.vector, 0.5)) {
    u->state.current = waiting;
    u->state.subject.vector = NULL;
    printf("switching to wait state\n");
  }
}
