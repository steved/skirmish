#include "ui/menu.h"
#include "ui/setup.h"

#ifdef HAVE_RUBY
#include "interface/rb_interface.h"
#endif

extern bool ruby_enabled;
ui_state menu_state = { &menu_render, &menu_update, &menu_handle_event, &menu_prepare, &menu_cleanup };

void menu_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  if(ruby_enabled)
    rb_interface_render(buffer);
}

void menu_update(camera *camera, PLAYERS *players) {
  if(ruby_enabled)
    rb_interface_update();
}

void menu_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {
  if(ruby_enabled)
    rb_interface_event(event);

  // needs to moved into rb_interface
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
    change_state(&setup_state);
}

void menu_prepare() {
  if(ruby_enabled)
    rb_interface_init("Menu");
}

void menu_cleanup() {
  if(ruby_enabled)
    rb_interface_cleanup();
}
