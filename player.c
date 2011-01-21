#include "player.h"

player *create_ai_player(int units) {
  player *ai = (player *) malloc(sizeof(player));
  ai->name = AI_NAMES[rand() % (sizeof(AI_NAMES) / sizeof(AI_NAMES[0]))];
  
  ai->num_units = units;
  ai->units = (unit **) malloc(sizeof(unit) * ai->num_units);
  
  return ai; 
}

player *create_human_player(const char *name, int units) {
  player *human = (player *) malloc(sizeof(player));
  human->name = name;

  human->num_units = units;
  human->units = (unit **) malloc(sizeof(unit) * human->num_units);

  return human;
}

void remove_player(player *player) {
  free(player->units);
  free(player);
}
