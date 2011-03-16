#ifdef HAVE_RUBY
#include "interface/rb_interface.h"

#include <ruby.h>
#include <assert.h>

// redefine Ruby/SDL's Surface struct
typedef struct {
  SDL_Surface *surface;
} Surface;

void rb_interface_load() {
  rb_require("interface/interface");
}

void rb_interface_init(char *state) {}
extern VALUE rb_cObject;
void rb_interface_remove() {}

void rb_interface_render(SDL_Surface *buffer) {
  // Get the SDL::Surface constant
  VALUE mSDL = rb_const_get(rb_cObject, rb_intern("SDL"));
  VALUE cSurface = rb_const_get(mSDL, rb_intern("Surface"));

  // Allocate the Surface and wrap it in SDL::Surface
  Surface *surf = ALLOC(Surface);
  surf->surface = buffer;
  VALUE obj = Data_Wrap_Struct(cSurface, 0, 0, surf);
  //Data_Get_Struct(obj, Surface, surf);
  
  // Draw!
  rb_funcall(rb_cObject, rb_intern("draw"), 1, obj);
  free(surf);
}

void rb_interface_event(SDL_Event event) {}
#endif
