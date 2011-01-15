#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"

#define WIDTH 640
#define HEIGHT 480
#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5

void poll_for_events();

const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
SDL_Surface *screen;
SDL_Event event;

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

  uint32_t next_game_tick = SDL_GetTicks();

  int loops;
  float interpolation;

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
    //display_game(interpolation);
  }

  return 0;
}

void poll_for_events() {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_MOUSEMOTION:
        printf("Mouse moved by %d,%d to (%d,%d)\n", 
            event.motion.xrel, event.motion.yrel,
            event.motion.x, event.motion.y);
        break;
      case SDL_MOUSEBUTTONDOWN:
        printf("Mouse button %d pressed at (%d,%d)\n",
            event.button.button, event.button.x, event.button.y);
        break;
      case SDL_QUIT:
        exit(0);
    }
  }
}
