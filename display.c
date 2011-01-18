#include "display.h"

SDL_Surface *display_game(float interpolation) {
  SDL_Surface *buffer = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 16, 0, 0, 0, 0);

  /* Print interpolation
  char buf[255];
  snprintf(buf, 255, "%f", interpolation);
  SDL_Surface *frames_per = draw_text(buf);
  int w,h;
  TTF_SizeUTF8(font, buf, &w, &h);
  SDL_Rect place = {WIDTH - w, HEIGHT - h};
  SDL_BlitSurface(frames_per, NULL, buffer, &place);
  */

  filledCircleColor(buffer, 100, 100, 100, 0xFF0000FF);

  return buffer;
}
