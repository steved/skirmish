#ifndef SKIRMISH_ATTR
#define SKIRMISH_ATTR

#include "util/linked_list.h"

#include <gsl/gsl_vector.h>

typedef struct attributes {
  int strength, speed, stamina;
  int health, armor;
  int experience;
} attributes;

typedef enum unit_type {
  infantry, cavalry, artillery
} unit_type;

typedef enum unit_weapon {
  none, bow, sword, spear
} unit_weapon;

// keeps primary and secondary weapon
// secondary weapon must always be unlimited
typedef struct weapons {
  enum unit_weapon primary_weapon;
  enum unit_weapon secondary_weapon;
  int primary_weapon_left;
} weapons;



typedef struct unit {
  attributes attributes;
  gsl_vector *vector;
  ll_node *state;
  unit_type type;
  weapons weapons;

  double display_radius[2];
  double collision_radius;

  union {
    ll_node *astar_node;
    gsl_vector *vector;
    struct unit *unit;
    struct unit **units;
    struct attack_state_data {
      ll_node *astar_node;
      struct unit *unit;
    } attacking;
  } state_data;

  struct division *division;
} unit;

#endif
