#include "main.h"
#include "camera.h"
#include "display.h"
#include "events.h"
#include "units.h"

#include "ui/game.h"
#include "ui/menu.h"
#include "ui/ui_state.h"

#include "util/file_load.h"
#include "util/random.h"
#include "util/text.h"

#include "units/romans.h"

#ifdef HAVE_RUBY
#include <ruby.h>
#include "interface/rb_interface.h"
#endif

#include <assert.h>

#define TICKS_PER_SECOND 25
#define MAX_FRAME_SKIP 5
const float SKIP_TICKS = 1000 / TICKS_PER_SECOND;
bool game_running = true;
ll_node *unit_types = NULL;

RUBY_GLOBAL_SETUP;

int main(int argc, char *argv[]) {
  unit_types = ll_add_to_bottom(unit_types, "legionary", &create_legionary);
  unit_types = ll_add_to_bottom(unit_types, "legionary_archer", &create_legionary_archer);
  unit_types = ll_add_to_bottom(unit_types, "legionary_cavalry", &create_legionary_cavalry);

  allocate_rng();

#ifdef HAVE_RUBY
  ruby_init();

  int dummy_argc = 2;
  char *dummy_argv[] = {"skirmish", "-e0"};
  ruby_process_options(dummy_argc, dummy_argv);
  
  ruby_script("skirmish");

  // initialize the load path and add the current working directory to it
  // this is 1.8 only apparently
  //ruby_init_loadpath();
  
  rb_eval_string("$: << Dir.getwd");
#endif

  current_state_mutex = SDL_CreateMutex();

  PLAYERS *players;
  char *filename;
  if(argc > 1) {
    filename = argv[1];
  } else {
    filename = "test.army";
  }
  printf("loading file %s\n", filename);
  players = read_file(filename);

  printf("creating camera\n");
  camera *camera = create_camera();

  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  } 
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  SDL_WM_SetCaption("Skirmish", "");
  SDL_WM_GrabInput(SDL_GRAB_ON);

  const SDL_VideoInfo *info = SDL_GetVideoInfo();
  if(!info) {
    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
    exit(1);
  }
  int bpp = info->vfmt->BitsPerPixel;

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, bpp, SDL_SWSURFACE);
  if(screen == 0) {
    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    exit(1);
  }

#ifdef HAVE_RUBY
  rb_define_global_const("WIDTH", INT2NUM(WIDTH));
  rb_define_global_const("HEIGHT", INT2NUM(HEIGHT));
  rb_define_global_const("BPP", INT2NUM(bpp));

  rb_interface_load();
#endif

  if(init_ttf() == -1) {
    fprintf(stderr, "Unable to set TTF.\n");
    exit(1);
  }

  change_state(&menu_state);

  uint32_t next_game_tick = SDL_GetTicks();

  int loops;
  float interpolation;

  while(game_running) {
    SDL_mutexP(current_state_mutex);
    assert(current_state != NULL);

    poll_for_events(camera, players, current_state);
    loops = 0;

    while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAME_SKIP) {
      current_state->update(camera, players);
      next_game_tick += SKIP_TICKS;
      loops++;
    }

    SDL_Surface *buffer = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, bpp, 0, 0, 0, 0xff);
    interpolation = (SDL_GetTicks() + SKIP_TICKS - next_game_tick) / SKIP_TICKS;
    current_state->render(buffer, camera, players, interpolation);

    SDL_mutexV(current_state_mutex);
    SDL_BlitSurface(buffer, NULL, screen, NULL);
    SDL_Flip(screen);
    SDL_FreeSurface(buffer);
  }

  printf("quitting...\n");

  SDL_KillThread(prepare_state_thread);
  SDL_DestroyMutex(current_state_mutex);
  current_state->cleanup();

  for(int i = 0; i < players->num; i++)
    remove_player(players->players[i]);
  free(players->players);
  free(players);
  remove_camera(camera);

  free_rng();

  SDL_FreeSurface(screen);
  close_ttf();
  SDL_Quit();


#ifdef HAVE_RUBY
  ruby_finalize();
#endif

  return 0;
}

void toggle_pause() {
  paused = !paused;
  if(paused)
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  else
    SDL_WM_GrabInput(SDL_GRAB_ON);
}
