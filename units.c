#include "camera.h"
#include "terrain.h"
#include "units.h"

#include <assert.h>
#include <gsl/gsl_vector.h>

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

void print_unit(unit *unit) {
  printf("type %d, cur_state %d, vector (%f, %f)\n", 
      unit->type, unit->state.current, 
      gsl_vector_get(unit->vector, 0), gsl_vector_get(unit->vector, 1));
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

gsl_vector *calculate_unit_display_position(unit *unit, camera *c, float interpolation) {
  return calculate_display_position(gsl_vector_get(unit->vector, 0), gsl_vector_get(unit->vector, 1),
      c);

  //if(unit->state.current == moving)
  //  gsl_vector_add_constant(pos, interpolation);
}

gsl_vector *calculate_display_position(double x, double y, camera *c) {
  gsl_vector *pos = gsl_vector_alloc(3);
  gsl_vector_set(pos, 0, x);
  gsl_vector_set(pos, 1, y);
  gsl_vector_scale(pos, 1.0f / ZOOM_LEVEL);
  gsl_vector_sub(pos, c->vector);
  gsl_vector_set(pos, 2, height_at(x, y));
  return pos;
}

gsl_vector *calculate_map_position(double x, double y, camera *c) {
  gsl_vector *pos = gsl_vector_alloc(3);
  gsl_vector_set(pos, 0, x);
  gsl_vector_set(pos, 1, y);
  gsl_vector_add(pos, c->vector);
  gsl_vector_scale(pos, ZOOM_LEVEL);
  gsl_vector_set(pos, 2, height_at(gsl_vector_get(pos, 0), gsl_vector_get(pos, 1)));
  return pos;
}

void remove_unit(unit *u) {
  free(u->vector);
  free(u);
}

void change_unit_state(unit *u, state_description desc, void *subj) {
  u->state.current = desc;
  u->state.subject.vector = (gsl_vector *) subj;
}
