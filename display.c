#include "display.h"
#include "selected.h"
#include "units.h"

#include "SDL_gfxPrimitives.h"

double unit_radius[][2] = {
  {1.5, 3}, {2.5, 2.5}, {2.5, 2.5}
};

void display_unit(SDL_Surface *surface, camera *camera, unit *unit, uint32_t color, float interpolation) { 
  gsl_vector *pos = calculate_display_position(unit, camera, interpolation);
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

    rad_x = unit_radius[unit->type][0] * mult;

    // special case for infantry since they're triangles
    if(unit->type == infantry)
      mult /= 2;
    rad_y = unit_radius[unit->type][1] * mult;
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
   circleColor(surface, x, y, rad_x + 5, (0xffffff00 - color) | 0x000000ff);
  }
}
