#ifndef SKIRMISH_UNITS
#define SKIRMISH_UNITS

#include "attributes.h"

unit *create_empty_unit();
void place(unit *, int, int);
void print_unit(unit *);
void print_attributes(attributes);
void print_weapons(weapons);

#endif
