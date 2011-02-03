#include "../display.h"
#include "game.h"
#include "../text.h"
#include "../units.h"

ui_state game_state = { &game_render, &game_update, &game_handle_event };

void game_render(SDL_Surface *buffer, camera *camera, player **players, int player_len) {
  for(int i = 0; i < player_len; i++) {
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
  SDL_Rect zo = { 0, HEIGHT - h, w, h };
  SDL_BlitSurface(zoom_surf, NULL, buffer, &zo);
  SDL_FreeSurface(zoom_surf);

  if(paused) {
    TTF_SizeUTF8(font, "PAUSED", &w, &h);
    SDL_Surface *paused_surf = draw_text("PAUSED");
    SDL_Rect paused_rect = { (WIDTH / 2) - (w / 2), (HEIGHT / 2) - (h / 2), w, h };

    SDL_Surface *overlay = SDL_CreateRGBSurface(SDL_SRCALPHA, WIDTH, HEIGHT, buffer->format->BitsPerPixel, 0, 0, 0, 0);
    SDL_FillRect(overlay, NULL, SDL_MapRGB(buffer->format, 0, 0, 0));
    SDL_SetAlpha(overlay, SDL_SRCALPHA, 128);

    SDL_BlitSurface(overlay, NULL, buffer, NULL);
    SDL_BlitSurface(paused_surf, NULL, buffer, &paused_rect);
    SDL_FreeSurface(overlay);
    SDL_FreeSurface(paused_surf);
  }

}

void game_update(player **players, int player_len, float interpolation) {

}

void game_handle_event(SDL_Event event, camera *camera) {

}
