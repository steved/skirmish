#include "romans.h"

unit *create_legionary_unit() {
  unit *unit = create_empty_unit();

  unit->attributes.speed = 3; // out of 10
  unit->attributes.strength = 10; // out of unlimited
  unit->attributes.stamina = 50; // out of 100
  unit->attributes.health = 60; // out of 100
  unit->attributes.armor = 25; // out of 100

  unit->weapons.primary_weapon = sword;

  print_unit(unit);

  return unit;
}

