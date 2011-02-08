#ifndef SKIRMISH_UI_STATE
#define SKIRMISH_UI_STATE

#include "../camera.h"
#include "../player.h"

typedef struct ui_state {
  void (*render)(SDL_Surface *, camera *, player **, int, float);
  void (*update)(player **, int);
  void (*handle_event)(SDL_Event, camera *);
  void (*prepare)();
  void (*cleanup)();
  bool preparing;
} ui_state;

extern ui_state *current_state;
void change_state(ui_state *next_state);

#endif
