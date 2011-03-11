#include "units/attack.h"

#include "units.h"

#include "util/random.h"

static int roll(int, int);
static unit_weapon current_weapon(unit *);

bool attack_unit(unit *attacker, unit *defender) {
  if(attacker->weapons.timer > 0) {
    attacker->weapons.timer--;
    return is_unit_dead(defender);
  }

  unit_weapon current = current_weapon(attacker);
  bool ranged = current == attacker->weapons.primary_weapon && attacker->weapons.primary_weapon_ranged;
  if(ranged)
    attacker->weapons.primary_weapon_left--;

  // reset weapon based on timer
  attacker->weapons.timer = 50;

  int attack_strength = roll(attacker->attributes.strength, attacker->attributes.experience);
  int defend_strength = roll(defender->attributes.armor, defender->attributes.experience);

  int damage = attack_strength - defend_strength;
  if(damage < 0)
    damage = 0;
  printf("damage to defender %d\n", damage);
  defender->attributes.health -= damage; 
  printf("defender has %d health left\n", defender->attributes.health);

  return is_unit_dead(defender);
}

static int roll(int size, int number) {
  int result = 1;
  for(int i = 0; i < number; i++) {
    result *= random_int_max(size) + 1; 
  }

  return result;
}

static unit_weapon current_weapon(unit *un) {
  unit_weapon type = un->weapons.primary_weapon;

  if(un->weapons.primary_weapon_ranged && un->weapons.primary_weapon_left <= 0) {
    type = un->weapons.secondary_weapon;
  }

  return type;
}

// TODO
int unit_range(unit *un) {
  unit_weapon current = current_weapon(un);
  bool ranged = current == un->weapons.primary_weapon && un->weapons.primary_weapon_ranged; 

  switch(current) {
    case sword:
      return un->collision_radius + 5;
      break;
    case bow:
      return un->collision_radius + un->attributes.strength * 100;
      break;
    case spear:
      return un->collision_radius + (ranged ? 25 : 10);
      break;
    default:
      return -1;
  }
}
