#include "unit_state_functions.h"

void (*state_functions[5])(unit *) = { &do_wait, &do_attack, &do_charge, &do_retreat, &do_move };

void do_wait(unit *u) {}
void do_attack(unit *u) {}
void do_charge(unit *u) {}
void do_retreat(unit *u) {}
void do_move(unit *u) {
  gsl_vector *go_to = gsl_vector_alloc(2);
  gsl_vector_memcpy(go_to, u->state.subject.vector);
  gsl_vector_sub(go_to, u->vector);
  gsl_vector_scale(go_to, 1.0f / 10);
  double max = gsl_vector_max(go_to);
  if(max < 1) {
    u->state.current = waiting;
    u->state.subject.vector = NULL;
    printf("switching to wait state\n");
  } else {
    gsl_vector_add(u->vector, go_to);
  }
  gsl_vector_free(go_to);
}
