#ifndef SKIRMISH_UNIT_STATE_FUNCTIONS
#define SKIRMISH_UNIT_STATE_FUNCTIONS

#include "attributes.h"
#include "camera.h"

// an 'associative' array for each state
// that defines which method is called for the unit
extern void (*state_functions[5])(PLAYERS *, camera *, unit *);

void do_wait(PLAYERS *, camera *, unit *);
void do_attack(PLAYERS *, camera *, unit *);
void do_charge(PLAYERS *, camera *, unit *);
void do_retreat(PLAYERS *, camera *, unit *);
void do_move(PLAYERS *, camera *, unit *);

#endif
