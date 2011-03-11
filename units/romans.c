#include "units/romans.h"
#include "units.h"

unit *create_legionary() {
  unit *unit = create_empty_unit();

  unit->attributes.speed = 1; // out of 10
  unit->attributes.strength = 10; // out of unlimited
  unit->attributes.stamina = 50; // out of 100
  unit->attributes.health = 60; // out of 100
  unit->attributes.armor = 5; // out of unlimited 

  unit->display_radius[0] = 1.5;
  unit->display_radius[1] = 3;

  unit->collision_radius = 4;

  unit->weapons.primary_weapon = spear;
  unit->weapons.primary_weapon_ranged = true;
  unit->weapons.primary_weapon_left = 10;

  unit->weapons.secondary_weapon = spear;

  return unit;
}

unit *create_legionary_archer() {
  unit *unit = create_empty_unit();
  unit->type = artillery;

  unit->attributes.speed = 1.1; // out of 10
  unit->attributes.strength = 5; // out of unlimited
  unit->attributes.stamina = 50; // out of 100
  unit->attributes.health = 45; // out of 100
  unit->attributes.armor = 0; // out of unlimited 

  unit->display_radius[0] = 1;
  unit->display_radius[1] = 1;

  unit->collision_radius = 4;

  unit->weapons.primary_weapon = bow;
  unit->weapons.primary_weapon_ranged = true;
  unit->weapons.primary_weapon_left = 95;

  unit->weapons.secondary_weapon = none;

  return unit;
}

unit *create_legionary_cavalry() {
  unit *unit = create_empty_unit();
  unit->type = artillery;

  unit->attributes.speed = 3; // out of 10
  unit->attributes.strength = 10; // out of unlimited
  unit->attributes.stamina = 90; // out of 100
  unit->attributes.health = 50; // out of 100
  unit->attributes.armor = 25; // out of unlimited 

  unit->display_radius[0] = 1;
  unit->display_radius[1] = 3;

  unit->collision_radius = 6;

  unit->weapons.primary_weapon = spear;
  unit->weapons.primary_weapon_ranged = false;

  return unit;
}
