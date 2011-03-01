#include "camera.h"
#include "collision.h"
#include "units.h"

#include "util/terrain.h"

#include <assert.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>

static void delta_height_scale(gsl_vector *, gsl_vector *);

unit *create_empty_unit() {
  unit *empty_unit = (unit *) malloc(sizeof(unit));
  assert(empty_unit != NULL);
  empty_unit->type = infantry;

  empty_unit->state.current = waiting;
  empty_unit->vector = gsl_vector_calloc(3);

  weapons empty = {none, none, 0};
  empty_unit->weapons = empty; 

  attributes attrs = {
    0, 0, 0, // strength, speed, stamina
    0, 0 // health, armor
  };

  empty_unit->attributes = attrs;

  return empty_unit;
}

void place(unit *unit, int x, int y) {
  gsl_vector_set(unit->vector, 0, x);
  gsl_vector_set(unit->vector, 1, y);
  gsl_vector_set(unit->vector, 2, height_at(x, y));
}

void place_at_vector(unit *unit, gsl_vector *v) {
  gsl_vector_memcpy(unit->vector, v);
  gsl_vector_free(v);
}

void print_unit(unit *unit) {
  printf("type %d, cur_state %d, vector (%f, %f, %f)\n", 
      unit->type, unit->state.current, 
      gsl_vector_get(unit->vector, 0), 
      gsl_vector_get(unit->vector, 1),
      gsl_vector_get(unit->vector, 2));
  printf("\t");
  print_attributes(unit->attributes);
  printf("\t");
  print_weapons(unit->weapons);
}

void print_attributes(attributes attributes) {
  printf("strength %d, speed %d, stamina %d, health %d, armor %d\n",
      attributes.strength, attributes.speed, attributes.stamina,
      attributes.health, attributes.armor);
}

void print_weapons(weapons weapons) {
  printf("primary weapon %d, left %d. secondary weapon %d\n",
      weapons.primary_weapon, weapons.primary_weapon_left,
      weapons.secondary_weapon);
}

void remove_unit(unit *u) {
  gsl_vector_free(u->vector);
  free(u);
}

void change_unit_state(unit *u, state_description desc, void *subj) {
  if(u->state.current == waiting)
    u->state.subject.vector = gsl_vector_alloc(3);

  u->state.current = desc;
  gsl_vector_memcpy(u->state.subject.vector, subj);
}

// defined in attributes.h
double unit_radius[] = {4, 3, 3};

bool check_for_unit_near(gsl_vector *location, camera *cam, PLAYERS *players, unit *unit_except) {
  for(int i = 0; i < players->num; i++) {
    player *pl = players->players[i];
    for(int j = 0; j < pl->num_divisions; j++) {
      division *div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        unit *un = div->units[k];

        if(un == unit_except)
          continue;

        if(bounding_circle_collision(location, unit_radius[unit_except->type], un->vector, unit_radius[un->type]))
          return true;
      }
    }
  }
  return false;
}

// returns true if @ destination, otherwise false
bool move_unit_towards(unit *subj, gsl_vector *dest, camera *camera, PLAYERS *players) {
  // set the z coordinate because it gets set incorrectly when
  // placing the unit in the beginning 
  gsl_vector_set(subj->vector, 2, height_at(gsl_vector_get(subj->vector, 0), gsl_vector_get(subj->vector, 1)));

  gsl_vector *go_to = gsl_vector_alloc(3);
  gsl_vector_memcpy(go_to, dest);
  gsl_vector_sub(go_to, subj->vector);

  double norm = gsl_blas_dnrm2(go_to);
  gsl_vector_scale(go_to, 1 / norm);

  delta_height_scale(go_to, subj->vector);
  gsl_vector_add(go_to, subj->vector);

  bool unit_at = check_for_unit_near(go_to, camera, players, subj);
  if(allowed_on_terrain(go_to) && !unit_at) {
    // find a way around?
    gsl_vector_memcpy(subj->vector, go_to);
  }

  gsl_vector_free(go_to);

  return bounding_circle_collision(subj->vector, unit_radius[subj->type], dest, 0.5);
}

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
}
