#include "division.h"
#include "collision.h"
#include "units.h"

#include "util/terrain.h"

#include <math.h>

static void advance_unit_position(float, int *, int *, int);

gsl_vector *place_division_at(division *div, int x, int y) {
  int unit_x, unit_y, rad;
  unit *u;

  int row_x = sin(div->structure.angle) * div->structure.row_padding;
  int row_y = cos(div->structure.angle) * div->structure.row_padding;

  unit_x = x;
  unit_y = y;

  for(int k = 0; k < div->size; k++) {
    if(k % div->structure.num_per_row == 0) {
      int l = k / div->structure.num_per_row;
      unit_x = x + (row_x * l);
      unit_y = y + (row_y * l);
    }

    u = div->units[k];
    rad = u->collision_radius * 2;
    place(u, unit_x, unit_y);

    while(!allowed_on_terrain(u->position)) {
      advance_unit_position(div->structure.angle, &unit_x, &unit_y, rad);
      place(u, unit_x, unit_y);
    }

    advance_unit_position(div->structure.angle, &unit_x, &unit_y, rad);
  }

  gsl_vector *end = gsl_vector_alloc(2);
  gsl_vector_set(end, 0, unit_x);
  gsl_vector_set(end, 1, unit_y);

  return end;
}

static void advance_unit_position(float angle, int *x, int *y, int rad) {
  int xdiff = cos(angle) * rad;
  *x += xdiff; 
  if(*x < 0) {
  } else if (*x > MAP_SIZE) {
  }

  int ydiff = sin(angle) * rad;
  *y += ydiff;
  if(*y < 0) {
  } else if(*y > MAP_SIZE) {
  }
}
