#include "attributes.h"
#include "player.h"
#include "selected.h"
#include "units.h"

#include <assert.h>

static const char *AI_NAMES[] = {
  "Hannibal", "Sun Tzu", "Ramesses II", "Ambiorix",
  "Leonidas", "Philip II of Macedon", "Alexander the Great",
  "David", "Hammurabi", "Nebuchadrezzar II", "Xerxes", "Darius", 
  "Marcus Licinius Crassus", "Julius Caesar", "Scipio Africanus"
};


player *create_ai_player(int units) {
  player *ai = (player *) malloc(sizeof(player));
  assert(ai != NULL);
  ai->name = AI_NAMES[rand() % (sizeof(AI_NAMES) / sizeof(AI_NAMES[0]))];
  ai->color = rand() | 0x00ff00ff;
  
  ai->num_units = units;
  ai->units = (unit **) malloc(sizeof(unit) * ai->num_units);
  assert(ai->units != NULL);
  
  return ai; 
}

player *create_human_player(const char *name, int units) {
  player *human = (player *) malloc(sizeof(player));
  assert(human != NULL);
  human->name = name;
  human->color = 0xff0000ff;

  human->num_units = units;
  human->units = (unit **) malloc(sizeof(unit) * human->num_units);
  assert(human->units != NULL);

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

void check_for_unit_at(camera *cam, player **players, int player_len, SDL_MouseButtonEvent button) {
  int eps = 3;
  if(button.button == 1) {
    unselect_all(); // eventually check for CTRL button to add to selections
    printf("looking for unit at (%d, %d)\n", button.x, button.y);
    for(int i = 0; i < player_len; i++) {
      player *pl = players[i];
      printf("checking player %d\n", i);
      for(int j = 0; j < pl->num_units; j++) {
        unit *un = pl->units[j];
        printf("checking unit %d\n", j);
        gsl_vector *pos = calculate_display_position(un, cam);
        double x = gsl_vector_get(pos, 0); double y = gsl_vector_get(pos, 1);
        printf("unit @ (%f, %f)\n", x, y);
        gsl_vector_free(pos);
        if((x >= button.x - eps && x <= button.x + eps) && (y >= button.y - eps && y <= button.y + eps)) {
          printf("found unit to select\n");
          select_unit(un);
          return;
        }
      }
    }
  }
  printf("could not find unit to select\n");
}
