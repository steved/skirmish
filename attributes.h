#ifndef SKIRMISH_ATTR
#define SKIRMISH_ATTR

#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>

typedef struct attributes {
  int strength, speed, stamina;
  int health, armor;
} attributes;

// to add: flanking, 
typedef enum state {
  waiting, attacking, charging, retreating
} state;

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
  state cur_state;
  unit_type type;
  weapons weapons;
} unit;

#endif
