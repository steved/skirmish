#include "display.h"

SDL_Surface *display_game(SDL_Surface *buffer, camera *camera, player **players, int len) {
  for(int i = 0; i < len; i++) {
    player *player = players[i];
    for(int j = 0; j < player->num_units; j++) {
      display_unit(buffer, camera, player->units[j], player->color);
    }
  }

  /*
  unit *u = create_empty_unit();
  place(u, 250, 250);
  display_unit(buffer, u);

  u->type = cavalry;
  place(u, 200, 250);
  display_unit(buffer, u);

  u->type = artillery;
  place(u, 250, 200);
  display_unit(buffer, u);

  free(u);
  */

  /* Print interpolation
  char buf[255];
  snprintf(buf, 255, "%f", interpolation);
  SDL_Surface *frames_per = draw_text(buf);
  int w,h;
  TTF_SizeUTF8(font, buf, &w, &h);
  SDL_Rect place = {WIDTH - w, HEIGHT - h};
  SDL_BlitSurface(frames_per, NULL, buffer, &place);
  SDL_FreeSurface(frames_per);
  */


  // display the title in the upper right
  SDL_Surface *title = draw_text("Skirmish");
  SDL_BlitSurface(title, NULL, buffer, NULL);
  SDL_FreeSurface(title);

  char zoom[7];
  snprintf(zoom, 7, "Zoom %i", ZOOM_LEVEL);
  int w,h;
  TTF_SizeUTF8(font, zoom, &w, &h);
  SDL_Surface *zoom_surf = draw_text(zoom);
  SDL_Rect zo = { 0, HEIGHT- h, WIDTH - (w * 7), h };
  SDL_BlitSurface(zoom_surf, NULL, buffer, &zo);
  SDL_FreeSurface(zoom_surf);

  return buffer;
}

double unit_radius[][2] = {
  {1.5, 3}, {2.5, 2.5}, {2.5, 2.5}
};

void display_unit(SDL_Surface *surface, camera *camera, unit *unit, uint32_t color) { 
  double x, y;
  x = gsl_vector_get(unit->vector, 0) - gsl_vector_get(camera->vector, 0);
  y = gsl_vector_get(unit->vector, 1) - gsl_vector_get(camera->vector, 1);

  double rad_x = unit_radius[unit->type][0];
  double rad_y = unit_radius[unit->type][1];

  if(x < -rad_x || y < -rad_y || x > WIDTH + rad_x || y > WIDTH + rad_y)
    return;

  rad_x *= 2;
  // special case for infantry since they're triangles
  rad_y *= (unit->type == infantry ? 1 : 2);

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

  // this unit is selected!
  //circleColor(surface, x, y, rad_x + 5, (0xffffff00 - color) | 0x000000ff);
}
