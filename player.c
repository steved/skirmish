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


player *create_ai_player(int divisions) {
  player *ai = (player *) malloc(sizeof(player));
  assert(ai != NULL);
  ai->name = AI_NAMES[rand() % (sizeof(AI_NAMES) / sizeof(AI_NAMES[0]))];
  ai->color = rand() | 0x00ff00ff;
  
  ai->num_divisions = divisions;
  ai->divisions = (division **) malloc(sizeof(division *) * ai->num_divisions);
  assert(ai->divisions != NULL);
  
  return ai; 
}

player *create_human_player(const char *name, int divisions) {
  player *human = (player *) malloc(sizeof(player));
  assert(human != NULL);
  human->name = name;
  human->color = 0xff0000ff;

  human->num_divisions = divisions;
  human->divisions = (division **) malloc(sizeof(division *) * human->num_divisions);
  assert(human->divisions != NULL);

  return human;
}

void remove_player(player *player) {
  int i, j;
  for(i = 0; i < player->num_divisions; i++) {
    division *div = player->divisions[i];
    for(j = 0; j < div->size; j++) {
      if(div->units[j] != NULL)
        remove_unit(div->units[j]);
    }
    free(div->units);
    free(div);
  }
  free(player->divisions);
  free(player);
}

void check_for_unit_at(camera *cam, player **players, int player_len, SDL_MouseButtonEvent button) {
  int eps = 3;
  bool modifier = (SDL_GetModState() & KMOD_CTRL) > 0;
  if(button.button != 1)
    return;

  if(!modifier) {
    unselect_all();
  }

  for(int i = 0; i < player_len; i++) {
    player *pl = players[i];
    for(int j = 0; j < pl->num_divisions; j++) {
      division *div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        unit *un = div->units[k];

        gsl_vector *pos = calculate_display_position(un, cam, 0);
        double x = gsl_vector_get(pos, 0);
        double y = gsl_vector_get(pos, 1);
        gsl_vector_free(pos);

        if((x >= button.x - eps && x <= button.x + eps) && (y >= button.y - eps && y <= button.y + eps)) {
          if(selected(un) && modifier) {
            unselect_unit(un);
          } else {
            if(ZOOM_LEVEL == 3 || ZOOM_LEVEL == 4) {
              select_division(div);
            } else {
              select_unit(un);
            }
          }
          return;
        }
      }
    }
  }
}
