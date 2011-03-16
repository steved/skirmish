#ifdef HAVE_RUBY
#include "interface/rb_interface.h"

#include <assert.h>
#include <ruby.h>
#include <stdbool.h>

static VALUE current_state_interface = Qnil;

extern VALUE rb_cObject;

#ifdef RUBY_1_9
extern void ruby_error_print();
#endif

extern bool ruby_enabled;

static VALUE require(VALUE args) {
  return rb_require("interface/interface");
}

void rb_interface_load() {
  int error;
  rb_protect(require, 0, &error);
  if(error) {
    printf("There was an error while loading the ruby interface, disabling.\n");
#ifdef RUBY_1_9
    ruby_error_print();
#endif
    ruby_enabled = false;
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

  if(error || TYPE(class) != T_CLASS) {
    printf("Error getting class %s\n", state);
#ifdef RUBY_1_9
    ruby_error_print();
#endif
    return;
  }

  VALUE instance = rb_protect(init, class, &error);

  if(error) {
    instance = Qnil;
    printf("Error initializing %s\n", state);
    rb_backtrace();
#ifdef RUBY_1_9
    ruby_error_print();
#endif
    return;
  }

  // set a class variable under object so that it gets GC-ed only when the state changes
  current_state_interface = rb_iv_set(rb_cObject, "current_state_interface", instance);
}

static VALUE cleanup(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("cleanup"), 0, NULL);
}

void rb_interface_cleanup() {
  if(current_state_interface == Qnil)
    return;

  int error;

  rb_protect(cleanup, Qnil, &error);

  if(error) {
    printf("Error cleaning up\n");
#ifdef RUBY_1_9
    ruby_error_print();
#endif
  }
}

static VALUE render(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("render"), 0, NULL);
};

void rb_interface_render(SDL_Surface *buffer) {
  if(current_state_interface == Qnil)
    return;

  int error;
  VALUE surface = rb_protect(render, Qnil, &error);
  if(error) {
    printf("Error rendering\n");
#ifdef RUBY_1_9
    ruby_error_print();
#endif
  } else {
    // convert the address provided from Ruby into a SDL_Surface pointer
    SDL_Surface *interface = (SDL_Surface *) NUM2ULL(surface);
    //SDL_DisplayFormatAlpha(interface);
    SDL_BlitSurface(interface, NULL, buffer, NULL);
  }
}

static VALUE event(VALUE args) {
  return Qnil;
}

void rb_interface_event(SDL_Event ev) {
  if(current_state_interface == Qnil)
    return;

  event(Qnil);
}

void rb_interface_update() {
  rb_gc();
}

#endif
