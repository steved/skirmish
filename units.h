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
void remove_unit(unit *);
unit *check_for_unit_near(gsl_vector *, PLAYERS *, unit *, bool, bool);
void update_unit(unit *, camera *, PLAYERS *);
void unit_dead(unit *);
bool is_unit_dead(unit *);

#endif
