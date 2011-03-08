#include "player.h"
#include "attributes.h"
#include "display.h"
#include "selected.h"
#include "units.h"

#include "util/random.h"

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
  ai->name = AI_NAMES[random_int_max((sizeof(AI_NAMES) / sizeof(AI_NAMES[0])))];
  ai->color = random_int() | 0x00ff00ff;
  ai->human = false;
  // XXX
  ai->column_padding = 5;
  
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
  // XXX
  human->column_padding = 5;

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
    free((char *) player->name);
  free(player);
}

bool select_units_at(bool modifier, int chk_x, int chk_y, camera *cam, PLAYERS *players) {
  if(!modifier)
    unselect_all();

  gsl_vector *v = calculate_map_position(chk_x, chk_y, cam);
  unit *nearest_unit = check_for_unit_near(v, players, NULL, true, false);
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
