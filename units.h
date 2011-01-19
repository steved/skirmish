#ifndef SKIRMISH_UNITS
#define SKIRMISH_UNITS

#include "attributes.h"

unit *create_empty_unit();
void place(unit *unit, int x, int y);
void print_unit(unit *unit);
void print_attributes(attributes attributes);
void print_weapons(weapons weapons);

#endif
