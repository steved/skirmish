#include "units/romans.h"
#include "units.h"

unit *create_legionary_unit() {
  unit *unit = create_empty_unit();

  unit->attributes.speed = 3; // out of 10
  unit->attributes.strength = 10; // out of unlimited
  unit->attributes.stamina = 50; // out of 100
  unit->attributes.health = 60; // out of 100
  unit->attributes.armor = 5; // out of unlimited 

  unit->display_radius[0] = 1.5;
  unit->display_radius[1] = 3;

  unit->collision_radius = 4;

  unit->weapons.primary_weapon = sword;

  return unit;
}

