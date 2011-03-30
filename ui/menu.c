#include "ui/menu.h"
#include "ui/setup.h"

#ifdef HAVE_RUBY
#include "interface/rb_interface.h"
static bool ui_init = false;
#endif

extern bool ruby_enabled;
ui_state menu_state = { &menu_render, &menu_update, &menu_handle_event, &menu_prepare, &menu_cleanup };

void menu_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
#ifdef HAVE_RUBY
  if(ruby_enabled && ui_init)
    rb_interface_render(buffer);
#endif
}

void menu_update(camera *camera, PLAYERS *players) {
#ifdef HAVE_RUBY
  if(ruby_enabled) {
    if(!ui_init) {
      rb_interface_init("Menu");
      ui_init = true;
    }
    rb_interface_update();
  } else {
#endif
  change_state(&setup_state);

#ifdef HAVE_RUBY
  }
#endif
}

void menu_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {
#ifdef HAVE_RUBY
  if(ruby_enabled && ui_init)
    rb_interface_event(event);
#endif

  // needs to moved into rb_interface
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
    change_state(&setup_state);
}

void menu_prepare() {
}

void menu_cleanup() {
#ifdef HAVE_RUBY
  if(ruby_enabled && ui_init) {
    rb_interface_cleanup();
    ui_init = false;
  }
#endif
}
