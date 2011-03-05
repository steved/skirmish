#ifndef SKIRMISH_UNITS
#define SKIRMISH_UNITS

#include "attributes.h"
#include "camera.h"
#include "player.h"

#include <gsl/gsl_vector.h>
#include <stdbool.h>

unit *create_empty_unit();
void place(unit *, int, int);
void place_at_vector(unit *, gsl_vector *);
void print_unit(unit *);
void print_attributes(attributes);
void print_weapons(weapons);
void remove_unit(unit *);
unit *check_for_unit_near(gsl_vector *, camera *, PLAYERS *, unit *, bool, bool);
bool move_unit_towards(unit *, gsl_vector *, camera *, PLAYERS *);
void update_unit(unit *, camera *, PLAYERS *);
bool attack_unit(unit *, unit *);
void unit_dead(unit *);

#endif
