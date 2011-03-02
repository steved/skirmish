#ifndef SKIRMISH_ATTR
#define SKIRMISH_ATTR

#include "util/linked_list.h"

#include <gsl/gsl_vector.h>

typedef struct attributes {
  int strength, speed, stamina;
  int health, armor;
} attributes;

// to add: flanking, 
typedef enum state_description {
  waiting, attacking, charging, retreating, moving
} state_description;

typedef struct state {
  state_description current;
  union {
    gsl_vector *vector;
    struct unit *unit;
    struct unit **units;
  } subject;
  ll_node *astar_node;
} state;

typedef enum unit_type {
  infantry, cavalry, artillery
} unit_type;

// an 'associative' array for each unit type
// that defines the radius of each unit in the x, y range
// for display purposes
extern double display_unit_radius[][2];
// same as above but for bounding circle collisions
extern double unit_radius[];

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
  state state;
  unit_type type;
  weapons weapons;
  struct division *division;
} unit;

#endif
