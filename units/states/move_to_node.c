#include "units/states/move_to_node.h"
#include "units/states/waiting.h"

#include "units.h"
#include "util/astar.h"

state move_to_node = { "move_to_node", &move_to_node_prepare, &move_to_node_update, &move_to_node_cleanup };

void move_to_node_prepare(unit *u, void *data) {
  u->state_data.astar_node = (ll_node *) data;
}

void move_to_node_cleanup(unit *u) {
  u->state_data.astar_node = ll_clear(u->state_data.astar_node);
}

bool move_to_node_update(PLAYERS *players, camera *camera, unit *u) {
  // if there is another move state pending
  // immediately switch to that
  if(u->state->next != NULL && u->state->next->value == &move_to_node) {
    return false;
  }

  gsl_vector *go_to = gsl_vector_calloc(3);

  ai_node *node = (ai_node *) u->state_data.astar_node->value;
  gsl_vector_set(go_to, 0, node->x);
  gsl_vector_set(go_to, 1, node->y);
  gsl_vector_set(go_to, 2, height_at(node->x, node->y));

  bool there = move_unit_towards(u, go_to, players); 
  gsl_vector_free(go_to);

  if(there) {
    ll_node *current = u->state_data.astar_node;
    if(current->next != NULL) {
      u->state_data.astar_node = ll_remove(current, current->value);
      there = false;
    } else {
      push_unit_state(u, &waiting, NULL);
    }
  }
  return !there;
}
