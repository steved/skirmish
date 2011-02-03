#include "unit_state_functions.h"

void (*state_functions[5])(unit *) = { &do_wait, &do_attack, &do_charge, &do_retreat, &do_move };

void do_wait(unit *u) {}
void do_attack(unit *u) {}
void do_charge(unit *u) {}
void do_retreat(unit *u) {}
void do_move(unit *u) {}
