#include "display.h"
#include "selected.h"
#include "units.h"

#include "util/astar.h"
#include "util/terrain.h"

#include "units/states/unit_state.h"

#include "SDL_gfxPrimitives.h"

gsl_vector *calculate_unit_display_position(unit *unit, camera *c, float interpolation) {
  gsl_vector *pos = calculate_display_position(x(unit->position), y(unit->position), c);
  //if(unit->state.current == moving)
  //  gsl_vector_scale(pos, 1 + (interpolation / 100));
  return pos;
}

gsl_vector *calculate_display_position(double x, double y, camera *c) {
  gsl_vector *pos = gsl_vector_alloc(3);
  gsl_vector_set(pos, 0, x);
  gsl_vector_set(pos, 1, y);
  gsl_vector_scale(pos, 1.0f / ZOOM_LEVEL);
  gsl_vector_sub(pos, c->vector);
  gsl_vector_set(pos, 2, height_at(x, y));
  return pos;
}

gsl_vector *calculate_map_position(double x, double y, camera *c) {
  gsl_vector *pos = gsl_vector_alloc(3);
  gsl_vector_set(pos, 0, x);
  gsl_vector_set(pos, 1, y);
  gsl_vector_add(pos, c->vector);
  gsl_vector_scale(pos, ZOOM_LEVEL);
  gsl_vector_set(pos, 2, height_at(gsl_vector_get(pos, 0), gsl_vector_get(pos, 1)));
  return pos;
}

void display_unit(SDL_Surface *surface, camera *camera, unit *unit, uint32_t color, float interpolation) { 
  if(is_unit_dead(unit))
    return;

  gsl_vector *pos = calculate_unit_display_position(unit, camera, interpolation);
  double x = gsl_vector_get(pos, 0);
  double y = gsl_vector_get(pos, 1);
  gsl_vector_free(pos);

  double rad_x, rad_y;
  if(ZOOM_LEVEL == 4) {
    rad_x = rad_y = 1;
  } else {
    double mult = 0.5;

    // mult * 2 for 2, mult * 4 for 1
    if(ZOOM_LEVEL == 1 || ZOOM_LEVEL == 2)
      mult *= (3 - ZOOM_LEVEL) * 2;

    rad_x = unit->display_radius[0] * mult;

    // special case for infantry since they're triangles
    if(unit->type == infantry)
      mult /= 2;
    rad_y = unit->display_radius[1] * mult;
  }

  if(x < -rad_x || y < -rad_y || x > WIDTH + rad_x || y > WIDTH + rad_y)
    return;

  if(ZOOM_LEVEL == 4) {
    filledCircleColor(surface, x, y, rad_x, color);
  } else {
    switch(unit->type) {
    case infantry:
      filledTrigonColor(surface, x, y - rad_y, x + rad_x, y + rad_y, x - rad_x, y + rad_y, color);
      break;
    case cavalry:
      filledCircleColor(surface, x, y, rad_x, color); 
      break;
    case artillery:
      boxColor(surface, x - rad_x, y - rad_y, x + rad_x, y + rad_y, color); 
    }
  }

  if(selected(unit)) {
   // this unit is selected!
   circleColor(surface, x, y, unit->collision_radius, (0xffffff00 - color) | 0x000000ff);
  }

#ifdef NAV_DEBUG
  // draw the AStar nodes if they exist
  if(unit->state != NULL && strcmp(((state *) unit->state->value)->name, "move_to_node") == 0) {
    ll_node *cur = unit->state_data.move_to_node.astar_node;
    if(cur == NULL)
      return;
    ai_node *node;
    gsl_vector *display;
    while(cur) {
      node = (ai_node *) cur->value;
      display = calculate_display_position(node->x, node->y, camera);
      circleColor(surface, gsl_vector_get(display, 0), gsl_vector_get(display, 1), 3, 0xff0000ff);
      free(display);
      cur = cur->next;
    }
  }
#endif
}
