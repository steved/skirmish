#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "units.h"
#include "events.h"
#include "display.h"

#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5

SDL_Surface *screen;
const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
