#ifdef HAVE_RUBY
#include "interface/rb_interface.h"

#include <assert.h>
#include <ruby.h>

static VALUE current_state_interface = Qnil;

extern VALUE rb_cObject;

static VALUE require(VALUE);
static VALUE render(VALUE);
extern void ruby_error_print();

static VALUE require(VALUE args) {
  return rb_require("interface/interface");
}

void rb_interface_load() {
  int error;
  rb_protect(require, 0, &error);
  if(error) {
    printf("There was an error while loading the ruby interface, disabling.\n");
    ruby_error_print();
#undef HAVE_RUBY
  }
}

static VALUE init(VALUE klass) {
  return rb_funcall(klass, rb_intern("new"), 0, NULL);
}

static VALUE get_class(VALUE klass) {
  return rb_const_get(rb_cObject, klass);
}

void rb_interface_init(char *state) {
  int error;
  VALUE class = rb_protect(get_class, rb_intern(state), &error);

  if(error) {
    printf("Error getting class %s\n", state);
    ruby_error_print();
    return;
  }

  VALUE instance = rb_protect(init, class, &error);

  if(error) {
    printf("Error initializing %s\n", state);
    ruby_error_print();
    return;
  }

  current_state_interface = rb_iv_set(rb_cObject, "current_state_interface", instance);
}

static VALUE cleanup(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("cleanup"), 0, NULL);
}

void rb_interface_cleanup() {
  int error;

  rb_protect(cleanup, Qnil, &error);

  if(error) {
    printf("Error cleaning up\n");
    ruby_error_print();
  }
}

static VALUE render(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("render"), 0, NULL);
};

void rb_interface_render(SDL_Surface *buffer) {
  int error;
  VALUE surface = rb_protect(render, Qnil, &error);
  if(error) {
    printf("Error rendering\n");
    ruby_error_print();
  } else if(FIXNUM_P(surface) && !NIL_P(surface)) {
    // convert the address provided from Ruby into a SDL_Surface pointer
    SDL_Surface *interface = (SDL_Surface *) NUM2ULONG(surface);
    //SDL_DisplayFormatAlpha(interface);
    SDL_BlitSurface(interface, NULL, buffer, NULL);
  } else {
    printf("Render either didn't return a fixnum or was nil. WRONG\n");
  }
}

static VALUE event(VALUE args) {
  return Qnil;
}

void rb_interface_event(SDL_Event ev) {
  event(Qnil);
}

void rb_interface_update() {
  rb_gc();
}

#endif
