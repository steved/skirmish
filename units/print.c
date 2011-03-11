#include "print.h"
#include "units/states/unit_state.h"

#include <stdio.h>

void print_unit(unit *unit) {
  printf("type %d, state %s, vector (%f, %f, %f)\n", 
      unit->type, 
      ((state *)unit->state->value)->name, 
      x(unit->position), 
      y(unit->position),
      z(unit->position));
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
