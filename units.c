#include "units.h"
#include "camera.h"
#include "collision.h"

#include "util/astar.h"
#include "util/random.h"
#include "util/terrain.h"

#include "units/states/waiting.h"

#include <assert.h>
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>

unit *create_empty_unit() {
  unit *empty_unit = (unit *) malloc(sizeof(unit));
  assert(empty_unit != NULL);
  empty_unit->type = infantry;

  empty_unit->state = NULL;
  push_unit_state(empty_unit, &waiting, NULL);

  empty_unit->position = gsl_vector_calloc(3);
  empty_unit->velocity = gsl_vector_calloc(3);
  empty_unit->heading = gsl_vector_calloc(3);
  empty_unit->side = gsl_vector_calloc(3);

  weapons empty = {none, none, false, 0, 0};
  empty_unit->weapons = empty; 

  attributes attrs = {
    0, 0, 0, // strength, speed, stamina
    0, 0, // health, armor
    1 // experience
  };

  empty_unit->display_radius[0] = 0;
  empty_unit->display_radius[1] = 0;
  empty_unit->collision_radius = 0;

  empty_unit->attributes = attrs;

  return empty_unit;
}

void place(unit *unit, int x, int y) {
  gsl_vector_set(unit->position, 0, x);
  gsl_vector_set(unit->position, 1, y);
  gsl_vector_set(unit->position, 2, height_at(x, y));
}

void place_at_vector(unit *unit, gsl_vector *v) {
  gsl_vector_memcpy(unit->position, v);
}

void remove_unit(unit *u) {
  gsl_vector_free(u->position);
  gsl_vector_free(u->velocity);
  gsl_vector_free(u->heading);
  gsl_vector_free(u->side);
  ll_clear(u->state);
  free(u);
}

unit *check_for_unit_near(gsl_vector *location, PLAYERS *players, unit *unit_except, bool only_human, bool only_enemies) {
  for(int i = 0; i < players->num; i++) {
    player *pl = players->players[i];

    if((only_human && !pl->human) || (only_enemies && pl->human))
      continue;

    for(int j = 0; j < pl->num_divisions; j++) {
      division *div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        unit *un = div->units[k];

        if(un == unit_except || is_unit_dead(un))
          continue;

        if(bounding_circle_collision(location, (unit_except == NULL ? un : unit_except)->collision_radius, un->position, un->collision_radius))
          return un;
      }
    }
  }
  return NULL;
}

// returns true if @ destination, otherwise false

void update_unit(unit *u, camera *cam, PLAYERS *players) {
  if(is_unit_dead(u))
    return;

  // set the velocity to 0
  gsl_vector_scale(u->velocity, 0);

  if(!((state *) u->state->value)->update(players, cam, u)) {
    pop_unit_state(u);
  }

  double norm = gsl_blas_dnrm2(u->velocity);
  if(norm > 0) {
    gsl_vector_scale(u->velocity, 1 / norm);

    gsl_vector_memcpy(u->heading, u->velocity);

    gsl_vector_scale(u->velocity, u->attributes.speed);
    gsl_vector_add(u->position, u->velocity);

    gsl_vector_set(u->side, 0, -y(u->heading));
    gsl_vector_set(u->side, 1, x(u->heading));
  }
}

void unit_dead(unit *un) {
  if(is_unit_dead(un))
    return;
  state *current_state = (state *) un->state->value;
  current_state->cleanup(un);

  un->state = ll_clear(un->state);
}

bool is_unit_dead(unit *un) {
  return un->state == NULL || un->attributes.health <= 0;
}

