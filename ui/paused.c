#include "ui/paused.h"

#include "display.h"
#include "util/text.h"

#ifdef HAVE_RUBY
#include "interface/rb_interface.h"
#endif

SDL_Surface *paused_surf, *overlay;
SDL_Rect paused_rect;
extern SDL_Surface *screen;
extern bool ruby_enabled;
static bool ui_init = false;
extern void toggle_pause();

ui_state paused_state = { &paused_render, &paused_update, &paused_handle_event, &paused_prepare, &paused_cleanup };

void paused_render(SDL_Surface *buffer, camera *camera, PLAYERS *players, float interpolation) {
  if(ruby_enabled && ui_init)
    rb_interface_render(buffer);
  else if(!ruby_enabled) {
    SDL_BlitSurface(overlay, NULL, buffer, NULL);
    SDL_BlitSurface(paused_surf, NULL, buffer, &paused_rect);
  }
}

void paused_update(camera *camera, PLAYERS *players) {
  if(ruby_enabled) {
    if(!ui_init) {
      rb_interface_init("Paused");
      ui_init = true;
    }
    rb_interface_update();
  }
}

void paused_handle_event(SDL_Event event, camera *camera, PLAYERS *players) {
  if(ruby_enabled && ui_init)
    rb_interface_event(event);
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
    toggle_pause();
}

void paused_prepare() {
  if(!ruby_enabled) {
    int w, h;
    TTF_SizeUTF8(font, "PAUSED", &w, &h);

    paused_rect.x = (WIDTH / 2) - (w / 2);
    paused_rect.y = (HEIGHT / 2) - (h / 2);
    paused_rect.w = w;
    paused_rect.h = h;

    paused_surf = draw_text("PAUSED");

    overlay = SDL_CreateRGBSurface(SDL_SRCALPHA, WIDTH, HEIGHT, screen->format->BitsPerPixel, 0, 0, 0, 0);
    SDL_FillRect(overlay, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_SetAlpha(overlay, SDL_SRCALPHA, 128);
  }
}

void paused_cleanup() {
  if(ruby_enabled && ui_init) {
    rb_interface_cleanup();
    ui_init = false;
  } else if(!ruby_enabled) {
    SDL_FreeSurface(overlay);
    SDL_FreeSurface(paused_surf);
  }
}
