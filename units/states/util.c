#include "units/states/util.h"

#include "units/states/move_to.h"
#include "units/states/move_to_node.h"

bool is_movement_state(ll_node *node) {
  if(node == NULL)
    return false;
  state *st = (state *) node->value;
  return st == &move_to || st == &move_to_node;
}
