#ifdef HAVE_RUBY
#include "interface/rb_interface.h"

#include <assert.h>
#include <ruby.h>

static VALUE require(VALUE);
static VALUE draw(VALUE);
extern void ruby_error_print();

void rb_interface_load() {
  int error;
  rb_protect(require, 0, &error);
  if(error) {
    printf("There was an error while loading the ruby interface, disabling.\n");
    ruby_error_print();
#undef HAVE_RUBY
  }
}

static VALUE require(VALUE arg) {
  return rb_require("interface/interface");
}

void rb_interface_init(char *state) {}
extern VALUE rb_cObject;
void rb_interface_remove() {}

void rb_interface_render(SDL_Surface *buffer) {
  int error;
  VALUE surface = rb_protect(draw, 0, &error);
  if(error) {
    ruby_error_print();
    printf("There was an error calling draw\n");
  } else if(FIXNUM_P(surface) && !NIL_P(surface)) {
    // convert the address provided from Ruby into a SDL_Surface pointer
    SDL_Surface *interface = (SDL_Surface *) NUM2ULONG(surface);
    //SDL_DisplayFormatAlpha(interface);
    SDL_BlitSurface(interface, NULL, buffer, NULL);
  } else {
    printf("Draw either didn't return a fixnum or was nil. WRONG\n");
  }
}

static VALUE draw(VALUE args) {
  return rb_funcall(rb_cObject, rb_intern("draw"), 0, NULL);
}

void rb_interface_event(SDL_Event event) {}
#endif
