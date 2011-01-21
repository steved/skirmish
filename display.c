#include "display.h"

SDL_Surface *display_game(float interpolation) {
  SDL_Surface *buffer = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 16, 0, 0, 0, 0);

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
  SDL_Rect zo = { 0, h, w * 7, h };
  SDL_BlitSurface(zoom_surf, NULL, buffer, &zo);
  SDL_FreeSurface(zoom_surf);

  //filledCircleColor(buffer, 100, 100, 100, 0xFF0000FF);

  return buffer;
}

void display_unit(SDL_Surface *surface, unit *unit) { 
  double x, y;
  x = gsl_vector_get(unit->vector, 0);
  y = gsl_vector_get(unit->vector, 1);

  switch(unit->type) {
    case infantry:
      filledTrigonColor(surface, x, y, x + 3, y + 6, x - 3, y + 6 , 0xff0000ff);
      break;
    case cavalry:
      filledCircleColor(surface, x, y, 5, 0xff0000ff); 
      break;
    case artillery:
      boxColor(surface, x - 5, y - 5, x + 5, y + 5, 0xff0000ff); 
  }
}
