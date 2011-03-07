#ifndef SKIRMISH_DISPLAY
#define SKIRMISH_DISPLAY

#include "SDL.h"
#include "attributes.h"
#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

void display_unit(SDL_Surface *, camera *, unit *, uint32_t, float);
gsl_vector *calculate_unit_display_position(unit *, camera *, float);
gsl_vector *calculate_display_position(double, double, camera *);
gsl_vector *calculate_map_position(double, double, camera *);

#endif
