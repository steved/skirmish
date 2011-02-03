#ifndef SKIRMISH_UI_STATE
#define SKIRMISH_UI_STATE

#include "../camera.h"
#include "../player.h"

typedef struct ui_state {
  void (*render)(SDL_Surface *, camera *, player *, int);
  void (*update)(player *, int);
  void (*handle_event)(SDL_Event, camera *);

} ui_state;

#endif
