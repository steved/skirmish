#ifndef SKIRMISH_ATTR
#define SKIRMISH_ATTR

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
} state;

typedef enum unit_type {
  infantry, cavalry, artillery
} unit_type;

// an 'associative' array for each unit type
// that defines the radius of each unit in the x, y range
extern double unit_radius[][2];

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
  int division_idx;
} unit;

// an 'associative' array for each state
// that defines which method is called for the unit
extern void (*state_functions[5])(unit *);

#endif
