#ifndef SKIRMISH_UI_STATE
#define SKIRMISH_UI_STATE

#include "../camera.h"
#include "../player.h"

#include "SDL_thread.h"

typedef struct ui_state {
  void (*render)(SDL_Surface *, camera *, player **, int, float);
  void (*update)(player **, int, camera *);
  void (*handle_event)(SDL_Event, camera *);
  void (*prepare)();
  void (*cleanup)();
} ui_state;

extern ui_state *current_state;
void change_state(ui_state *next_state);
SDL_mutex *current_state_mutex;

#endif
