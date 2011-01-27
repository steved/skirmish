#ifndef SKIRMISH_MAIN
#define SKIRMISH_MAIN

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "SDL_rotozoom.h"

#include "player.h"
#include "units.h"
#include "events.h"
#include "display.h"
#include "romans.h"
#include "terrain.h"

#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5

SDL_Surface *screen;
const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
player *human;
bool paused = false;
void toggle_pause();

#endif
