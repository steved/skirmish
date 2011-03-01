#include "attributes.h"
#include "display.h"
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
  ai->human = false;
  
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
  human->human = true;

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
  if(player->human)
    free(player->name);
  free(player);
}

bool check_for_unit_at(bool modifier, int chk_x, int chk_y, camera *cam, PLAYERS *players) {
  if(!modifier)
    unselect_all();

  gsl_vector *v = calculate_map_position(chk_x, chk_y, cam);
  unit *nearest_unit = get_unit_near(v, cam, players);
  gsl_vector_free(v);

  if(nearest_unit == NULL) {
    return false;
  }

  bool unit_selected = selected(nearest_unit);
  if(unit_selected && modifier) {
    unselect_unit(nearest_unit);
  } else if(ZOOM_LEVEL == 3 || ZOOM_LEVEL == 4) {
    select_division(nearest_unit->division);
  } else {
    select_unit(nearest_unit);
  }

  return true;
}

unit *get_unit_near(gsl_vector *v, camera *camera, PLAYERS *players) {
  int eps = 3 * ZOOM_LEVEL;

  gsl_vector *diff = gsl_vector_calloc(3);

  for(int i = 0; i < players->num; i++) {
    player *pl = players->players[i];
    for(int j = 0; j < pl->num_divisions; j++) {
      division *div = pl->divisions[j];
      for(int k = 0; k < div->size; k++) {
        unit *un = div->units[k];

        gsl_vector_memcpy(diff, v);
        gsl_vector_sub(diff, un->vector);

        double min, max;
        gsl_vector_minmax(diff, &min, &max);

        if(min >= -eps && max <= eps) {
          gsl_vector_free(diff);
          return un;
        }
      }
    }
  }

  gsl_vector_free(diff);
  return NULL;
}
