#include "units/states/follow.h"
#include "units/states/move_to.h"
#include "units/states/move_to_node.h"
#include "units/states/waiting.h"

#include "units/move.h"

#include <gsl/gsl_blas.h>
#include <math.h>

state follow = { "follow", &follow_prepare, &follow_update, &follow_cleanup };

void follow_prepare(unit *un, void *data) {
  ll_node *head = (ll_node *) data;

  unit *leader = (unit *) head->value;
  gsl_vector *offset = gsl_vector_calloc(3);
  gsl_vector_memcpy(offset, leader->position);
  gsl_vector_sub(offset, un->position);

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
  gsl_vector_memcpy(next_pos, leader->position);
  gsl_vector_add(next_pos, leader->velocity);
  gsl_vector_sub(next_pos, un->state_data.following.offset);

  bool there = move_unit_towards(un, next_pos, players);
  gsl_vector_free(next_pos);

  if(leader->state->value == &move_to_node || leader->state->value == &move_to) {
    double norm;
    gsl_vector *dest = gsl_vector_calloc(3);
    if(leader->state->value == &move_to_node)
      gsl_vector_memcpy(dest, leader->state_data.move_to_node.vector);
    else
      gsl_vector_memcpy(dest, leader->state_data.vector);
    norm = gsl_blas_dnrm2(dest);
    gsl_vector_scale(dest, 1 / norm);
    gsl_vector_add(next_pos, dest);
    gsl_vector_free(dest);

    unit *neighbor;
    ll_node *node = un->state_data.following.neighbors;
    gsl_vector *separation = gsl_vector_calloc(3);

    double radius;
    while(node) {
      gsl_vector_memcpy(separation, un->position);
      neighbor = (unit *) node->value;
      gsl_vector_sub(separation, neighbor->position);
      radius = neighbor->collision_radius + un->collision_radius;
      if(neighbor != un && abs(lround(x(separation))) <= radius && abs(lround(y(separation))) <= radius) {
        gsl_vector_sub(separation, neighbor->velocity);
        gsl_vector_scale(separation, neighbor->collision_radius + un->collision_radius);
        norm = gsl_blas_dnrm2(separation);
        gsl_vector_scale(separation, 1 / norm);
        gsl_vector_add(un->velocity, separation);
      }
      node = node->next;
    }

    gsl_vector_memcpy(separation, un->position);
    gsl_vector_sub(separation, leader->position);
    radius = leader->collision_radius + un->collision_radius;
    if(abs(lround(x(separation))) <= radius && abs(lround(y(separation))) <= radius) {
      gsl_vector_sub(separation, leader->velocity);
      gsl_vector_scale(separation, leader->collision_radius + un->collision_radius);
      norm = gsl_blas_dnrm2(separation);
      gsl_vector_scale(separation, 1 / norm);
      gsl_vector_add(un->velocity, separation);
    }

    
    /*for(int p = 0; p < players->num; p++) {
      player *pl = players->players[p];
      for(int d = 0; d < pl->num_divisions; d++) {
        division *div = pl->divisions[d];
        for(int u = 0; u < div->size; u++) {
          unit *unit_to_check = div->units[u];
          gsl_vector_memcpy(separation, un->position);
          //if(!ll_include(un->state_data.following.neighbors, unit_to_check)) {
          if(unit_to_check != un) {
            gsl_vector_sub(separation, unit_to_check->position);
            gsl_vector_scale(separation, unit_to_check->collision_radius);
            norm = gsl_blas_dnrm2(separation);
            gsl_vector_scale(separation, 1 / norm);
            gsl_vector_scale(separation, 1 / norm);
            gsl_vector_add(next_pos, separation);
          }
        }
      }
    }*/
    gsl_vector_free(separation);
  }


  return leader->state->value == &move_to_node || leader->state->value == &move_to || !there;
}
