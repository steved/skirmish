#include "display.h"
#include "selected.h"
#include "units.h"

#include "util/terrain.h"

#include "SDL_gfxPrimitives.h"

gsl_vector *calculate_unit_display_position(unit *unit, camera *c, float interpolation) {
  gsl_vector *pos = calculate_display_position(gsl_vector_get(unit->vector, 0), gsl_vector_get(unit->vector, 1), c);
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

double display_unit_radius[][2] = {
  {1.5, 3}, {2.5, 2.5}, {2.5, 2.5}
};

void display_unit(SDL_Surface *surface, camera *camera, unit *unit, uint32_t color, float interpolation) { 
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

    rad_x = display_unit_radius[unit->type][0] * mult;

    // special case for infantry since they're triangles
    if(unit->type == infantry)
      mult /= 2;
    rad_y = display_unit_radius[unit->type][1] * mult;
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
   circleColor(surface, x, y, unit_radius[unit->type], (0xffffff00 - color) | 0x000000ff);
  }
}
