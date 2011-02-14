#ifndef SKIRMISH_UNITS
#define SKIRMISH_UNITS

#include "camera.h"
#include "attributes.h"

unit *create_empty_unit();
void place(unit *, int, int);
void print_unit(unit *);
void print_attributes(attributes);
void print_weapons(weapons);
gsl_vector *calculate_unit_display_position(unit *, camera *, float);
gsl_vector *calculate_display_position(double, double, camera *);
gsl_vector *calculate_map_position(double, double, camera *);
void remove_unit(unit *);
void change_unit_state(unit *, state_description, void *);

#endif
