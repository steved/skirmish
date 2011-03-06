#include "follow.h"

state follow = { "follow", &follow_prepare, &follow_update, &follow_cleanup };

void follow_prepare(unit *un, void *data) {
  ll_node *head = (ll_node *) data;
  un->state_data.following.leader = (unit *) head->value;
  un->state_data.following.neighbors = head->next;
}

void follow_cleanup(unit *un) {
  un->state_data.following.leader = NULL;
  un->state_data.following.neighbors = NULL;
}

bool follow_update(PLAYERS *players, camera *cam, unit *un) {
  //offset pursuit

  return true;
}
