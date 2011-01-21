#include "main.h"
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL)); // seed the random

  remove_player(create_ai_player(10));

  player *human = create_human_player("Steven Davidovitz", 2);
  human->units[0] = create_legionary_unit();
  place(human->units[0], 100, 100);
  human->units[1] = create_legionary_unit();
  place(human->units[1], 50, 100);

  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  } 
  atexit(SDL_Quit);

  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  if(!info) {
    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
    exit(1);
  }
  int bpp = info->vfmt->BitsPerPixel;

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, bpp, SDL_SWSURFACE);
  if(screen == 0) {
    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    exit(1);
  }

  if(init_ttf() == -1) {
    fprintf(stderr, "Unable to set TTF.\n");
    exit(1);
  }

  uint32_t next_game_tick = SDL_GetTicks();

  int loops;
  float interpolation;

  bool game_running = true;
  while(game_running) {
    poll_for_events();
    loops = 0;

    while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAME_SKIP) {
      //update_game(interpolation);
      next_game_tick += SKIP_TICKS;
      loops++;
    }

    interpolation = (SDL_GetTicks() + SKIP_TICKS - next_game_tick) / SKIP_TICKS;
    SDL_Surface *buffer = display_game(human->units, 2);
    SDL_BlitSurface(buffer, NULL, screen, NULL);
    SDL_FreeSurface(buffer);
    SDL_Flip(screen);
  }


  remove_player(human);

  SDL_FreeSurface(screen);
  return 0;
}
