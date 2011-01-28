#include "camera.h"
#include "units.h"

#include <assert.h>
#include <gsl/gsl_vector.h>

unit *create_empty_unit() {
  unit *empty_unit = (unit *) malloc(sizeof(unit));
  assert(empty_unit != NULL);
  empty_unit->type = infantry;

  empty_unit->cur_state = waiting;
  empty_unit->vector = gsl_vector_calloc(2);

  weapons empty = {none, none, 0};
  empty_unit->weapons = empty; 

  attributes attrs = {
    0, 0, 0, // strength, speed, stamina
    0, 0 // health, armor
  };

  empty_unit->attributes = attrs;

  //print_unit(empty_unit);

  return empty_unit;
}

void place(unit *unit, int x, int y) {
  gsl_vector_set(unit->vector, 0, x);
  gsl_vector_set(unit->vector, 1, y);
}

void print_unit(unit *unit) {
  printf("type %d, cur_state %d, vector (%f, %f)\n", 
      unit->type, unit->cur_state, 
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

gsl_vector *calculate_display_position(unit *unit, camera *c) {
  gsl_vector *pos = gsl_vector_alloc(2);
  gsl_vector_memcpy(pos, unit->vector);
  gsl_vector_scale(pos, 1.0f / ZOOM_LEVEL);
  gsl_vector_sub(pos, c->vector);
  return pos;
}
