#ifndef SKIRMISH_UNITS
#define SKIRMISH_UNITS

#include "camera.h"
#include "attributes.h"

unit *create_empty_unit();
void place(unit *, int, int);
void print_unit(unit *);
void print_attributes(attributes);
void print_weapons(weapons);
gsl_vector *calculate_display_position(unit *, camera *, float);
void remove_unit(unit *);

#endif
