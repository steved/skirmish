#include "units/attack.h"

#include "util/random.h"

static int roll(int, int);

bool attack_unit(unit *attacker, unit *defender) {
  int attack_strength = roll(attacker->attributes.strength, attacker->attributes.experience);
  int defend_strength = roll(defender->attributes.armor, defender->attributes.experience);

  int damage = attack_strength - defend_strength;
  if(damage < 0)
    damage = 0;
  printf("damage to defender %d\n", damage);
  defender->attributes.health -= damage; 
  printf("defender has %d health left\n", defender->attributes.health);

  return defender->attributes.health <= 0;
}

static int roll(int size, int number) {
  int result = 1;
  for(int i = 0; i < number; i++) {
    result *= random_int_max(size) + 1; 
  }

  return result;
}

// TODO
int unit_range(unit *un) {
  return un->collision_radius + 5;
}


