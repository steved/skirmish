#include "main.h"

int main(int argc, char *argv[]) {
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

  unit *u = create_empty_unit();

  bool game_running = true;
  while(game_running) {
    poll_for_events();
    loops = 0;

    while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAME_SKIP) {
      //update_game();
      next_game_tick += SKIP_TICKS;
      loops++;
    }

    interpolation = (SDL_GetTicks() + SKIP_TICKS - next_game_tick) / SKIP_TICKS;
    SDL_Surface *buffer = display_game(interpolation);
    SDL_BlitSurface(buffer, NULL, screen, NULL);
    SDL_Flip(screen);
  }

  return 0;
}

