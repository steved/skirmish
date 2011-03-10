#include "units/states/follow.h"
#include "units/states/move_to.h"
#include "units/states/move_to_node.h"
#include "units/states/waiting.h"

#include "units.h"

#include <gsl/gsl_blas.h>
#include <math.h>

state follow = { "follow", &follow_prepare, &follow_update, &follow_cleanup };

void follow_prepare(unit *un, void *data) {
  ll_node *head = (ll_node *) data;

  unit *leader = (unit *) head->value;
  gsl_vector *offset = gsl_vector_calloc(3);
  gsl_vector_memcpy(offset, leader->vector);
  gsl_vector_sub(offset, un->vector);

  un->state_data.following.leader = leader;
  un->state_data.following.offset = offset;
  un->state_data.following.neighbors = head->next;
  
  push_unit_state(un, &waiting, NULL);
}

void follow_cleanup(unit *un) {
  un->state_data.following.leader = NULL;
  un->state_data.following.neighbors = NULL;

  gsl_vector_free(un->state_data.following.offset);
  un->state_data.following.offset = NULL;
}

bool follow_update(PLAYERS *players, camera *cam, unit *un) {
  unit *leader = un->state_data.following.leader;

  gsl_vector *next_pos = gsl_vector_calloc(3);
  gsl_vector_memcpy(next_pos, leader->vector);
  gsl_vector_sub(next_pos, un->state_data.following.offset);

  if(leader->state->value == &move_to_node || leader->state->value == &move_to) {
    unit *neighbor;
    ll_node *node = un->state_data.following.neighbors;
    gsl_vector *separation = gsl_vector_calloc(3);

    while(node) {
      gsl_vector_memcpy(separation, un->vector);
      neighbor = (unit *) node->value;
      if(neighbor != un) {
        gsl_vector_sub(separation, neighbor->vector);
        gsl_vector_add_constant(separation, -neighbor->collision_radius);
        double norm = gsl_blas_dnrm2(separation);
        gsl_vector_scale(separation, 1 / norm);
        gsl_vector_add(next_pos, separation);
      }
      node = node->next;
    }
    gsl_vector_free(separation);
  }

  bool there = move_unit_towards(un, next_pos, players);
  gsl_vector_free(next_pos);

  return leader->state->value == &move_to_node || leader->state->value == &move_to || !there;
}
