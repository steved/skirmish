#ifdef HAVE_RUBY
#include "interface/rb_interface.h"
#include "util/text.h"

#include <assert.h>
#include <ruby.h>
#include <stdbool.h>

static VALUE current_state_interface = Qnil;
extern VALUE rb_cObject;
extern bool ruby_enabled;
extern TTF_Font *font;
VALUE o_font = 0;

#ifdef RUBY_1_9
extern VALUE rb_errinfo();
static void rb_interface_backtrace();
#define BACKTRACE rb_interface_backtrace()
#elif RUBY_1_8
#define BACKTRACE rb_backtrace()
#endif

// Initializes a Ruby class with pointer as an argument to #new
// num is the number of class arguments following
//
// To an initialize an FFI::Pointer where ptr is an Ruby Fixnum
// initialize_class(ptr, 2, "FFI", "Pointer");
static VALUE initialize_class(VALUE pointer, int num, ...) {
  va_list classes;

  ID klass_name;
  VALUE klass = rb_cObject;

  va_start(classes, num);
  for(int i = 0; i < num; i++) {
    klass_name = rb_intern(va_arg(classes, char *));
    klass = rb_const_get(klass, klass_name);
  }
  va_end(classes);

  return rb_funcall(klass, rb_intern("new"), 1, pointer);
}

// FFI::Pointer.new(ptr)
// ptr is converted from an unsigned long long to a Ruby Fixnum
static VALUE initialize_ffi_pointer(void *ptr) {
  VALUE pointer = ULL2NUM((unsigned long long) ptr);
  return initialize_class(pointer, 2, "FFI", "Pointer");
}

// Ruby equivalent of object.pointer.autopointer = false
static void set_autopointer_false(VALUE object) {
  VALUE o_pointer = rb_funcall(object, rb_intern("pointer"), 0, NULL);
  rb_funcall(o_pointer, rb_intern("autorelease="), 1, Qfalse);
}

static VALUE require(VALUE args) {
  return rb_require("interface/interface");
}

void rb_interface_load() {
  int error;
  rb_protect(require, 0, &error);
  if(error) {
    printf("There was an error while loading the ruby interface, disabling.\n");
    BACKTRACE;
    ruby_enabled = false;
  }
}

//static VALUE init(VALUE klass) {
//  return rb_funcall(klass, rb_intern("new"), 0, NULL);
//}

static VALUE get_class(VALUE klass) {
  return rb_const_get(rb_cObject, klass);
}

void rb_interface_init(char *state) {
  int error;
  VALUE class = rb_protect(get_class, rb_intern(state), &error);

  if(error || TYPE(class) != T_CLASS) {
    printf("Error getting class %s\n", state);
    BACKTRACE;
    return;
  }
  error = 0;

  if(font != NULL && o_font == 0) {
    o_font = initialize_class(initialize_ffi_pointer(font), 3, "SDL", "TTF", "Font");
    set_autopointer_false(o_font);
  } else if(font == NULL) {
    o_font = Qnil;
  }

  VALUE instance = rb_funcall(class, rb_intern("new"), 1, o_font);
  //VALUE instance = rb_protect(init, class, &error);

  if(error) {
    instance = Qnil;
    printf("Error initializing %s\n", state);
    BACKTRACE;
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
    BACKTRACE;
  }
}

static VALUE render(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("render"), 1, args);
};

void rb_interface_render(SDL_Surface *buffer) {
  if(current_state_interface == Qnil)
    return;

  int error;

  // Initialize a new FFI::Pointer and pass it to SDL::Surface.new
  VALUE o_surface = initialize_class(initialize_ffi_pointer(buffer), 2, "SDL", "Surface");

  // We're going to manage the underlying pointer for the SDL::Surface
  // In ruby: surface.pointer.autorelease = false
  set_autopointer_false(o_surface);

  // Render
  rb_protect(render, o_surface, &error);

  if(error) {
    printf("Error rendering\n");
    BACKTRACE;
  }
}

static VALUE event(VALUE args) {
  return rb_funcall(current_state_interface, rb_intern("event"), 1, args);
}

void rb_interface_event(SDL_Event ev) {
  if(current_state_interface == Qnil)
    return;

  // Get the required classes -
  // FFI::Pointer and SDL::Event
  int error;

  // Initialize a new FFI::Pointer and pass it to SDL::Event.new
  // Call SDL::Event#unwrap which returns the specific Event class and pass it to the state
  VALUE o_event = initialize_class(initialize_ffi_pointer(&ev), 2, "SDL", "Event");
  VALUE o_unwrapped = rb_funcall(o_event, rb_intern("unwrap"), 0, NULL);
  
  rb_protect(event, o_unwrapped, &error);

  if(error) {
    printf("There was an error with event handling\n");
    BACKTRACE;
  }
}

void rb_interface_update() {
  rb_gc();
}

#ifdef RUBY_1_9
static void rb_interface_backtrace() {
  if(NIL_P(rb_errinfo()))
    return;

  VALUE ruby_errinfo = rb_errinfo();
  VALUE ary = rb_funcall(ruby_errinfo, rb_intern("backtrace"), 0);

  for (int c = 0; c < RARRAY_LEN(ary); c++) {
    printf("\tfrom %s\n", RSTRING_PTR(RARRAY_PTR(ary)[c]));
  }
}
#endif

#endif
