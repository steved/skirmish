#include "units/states/attack.h"
#include "units/states/waiting.h"

#include "collision.h"
#include "units.h"

#include "util/astar.h"

state attack = { "attack", &attack_prepare, &attack_update, &attack_cleanup };

void attack_prepare(unit *u, void *data) {
  u->state_data.attacking.unit = (unit *) data;
  u->state_data.attacking.astar_node = NULL;
}

void attack_cleanup(unit *u) {
  u->state_data.attacking.unit = NULL;
  u->state_data.attacking.astar_node = ll_clear(u->state_data.attacking.astar_node);
}

bool attack_update(PLAYERS *players, camera *cam, unit *u) {
  unit *defender = u->state_data.attacking.unit;
  ll_node *current = u->state_data.attacking.astar_node;

  if(bounding_circle_collision(u->position, unit_range(u), 
        defender->position, defender->collision_radius)) {
    u->state_data.attacking.astar_node = ll_clear(u->state_data.attacking.astar_node);
    bool dead = attack_unit(u, defender);
    if(dead) {
      unit_dead(defender);
      push_unit_state(u, &waiting, NULL);
      return false;
    }
  } else if(current != NULL) {
    gsl_vector *go_to = gsl_vector_calloc(3);

    ai_node *node = (ai_node *) u->state_data.attacking.astar_node->value;
    gsl_vector_set(go_to, 0, node->x);
    gsl_vector_set(go_to, 1, node->y);
    gsl_vector_set(go_to, 2, height_at(node->x, node->y));

    move_unit_towards(u, go_to, players); 
    gsl_vector_free(go_to);

    if(current->next != NULL) {
      u->state_data.attacking.astar_node = ll_remove(current, current->value);
    }
  } else {
    printf("recalculating\n");
    u->state_data.attacking.astar_node = ll_clear(current);
    u->state_data.attacking.astar_node = shortest_path(players, u, defender->position);
  }

  return true;
}
