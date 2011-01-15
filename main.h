#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#define WIDTH 640
#define HEIGHT 480
#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5

void poll_for_events();
void handle_keypress(int key);

const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
SDL_Surface *screen;
SDL_Event event;
