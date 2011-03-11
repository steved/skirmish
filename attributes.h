#ifndef SKIRMISH_ATTR
#define SKIRMISH_ATTR

#include "util/linked_list.h"

#include <gsl/gsl_vector.h>

#define x(v) gsl_vector_get(v, 0)
#define y(v) gsl_vector_get(v, 1)
#define z(v) gsl_vector_get(v, 2)

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

struct attack_state_data {
  ll_node *astar_node;
  struct unit *unit;
};

struct follow_state_data {
  ll_node *neighbors;
  struct unit *leader;
  gsl_vector *offset;
};

struct move_to_node_data {
  ll_node *astar_node;
  gsl_vector *vector; 
};

typedef struct unit {
  attributes attributes;

  gsl_vector *position, *heading, *side, *velocity;
  double max_speed;

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
    struct follow_state_data following;
    struct attack_state_data attacking;
    struct move_to_node_data move_to_node;
  } state_data;

  struct division *division;
} unit;

#endif
