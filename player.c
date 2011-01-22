#include "player.h"

static const char *AI_NAMES[] = {
  "Hannibal", "Sun Tzu", "Ramesses II", "Ambiorix",
  "Leonidas", "Philip II of Macedon", "Alexander the Great",
  "David", "Hammurabi", "Nebuchadrezzar II", "Xerxes", "Darius", 
  "Marcus Licinius Crassus", "Julius Caesar", "Scipio Africanus"
};


player *create_ai_player(int units) {
  player *ai = (player *) malloc(sizeof(player));
  ai->name = AI_NAMES[rand() % (sizeof(AI_NAMES) / sizeof(AI_NAMES[0]))];
  ai->color = rand() | 0x00ff00ff;
  
  ai->num_units = units;
  ai->units = (unit **) malloc(sizeof(unit) * ai->num_units);
  
  return ai; 
}

player *create_human_player(const char *name, int units) {
  player *human = (player *) malloc(sizeof(player));
  human->name = name;
  human->color = 0xff0000ff;

  human->num_units = units;
  human->units = (unit **) malloc(sizeof(unit) * human->num_units);

  return human;
}

void remove_player(player *player) {
  for(int i = 0; i < player->num_units; i++) {
    if(player->units[i] != NULL)
      gsl_vector_free(player->units[i]->vector);
  }
  free(player->units);
  free(player);
}
