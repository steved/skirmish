#ifndef SKIRMISH_EVENTS
#define SKIRMISH_EVENTS

#include "camera.h"
#include <stdbool.h>

void poll_for_events(camera *);
extern void toggle_pause();
extern bool paused;

#endif
