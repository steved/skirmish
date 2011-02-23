#include "camera.h"
#include "display.h"

#include "util/terrain.h"

#include <assert.h>

#define ZOOM_MIN 1
#define ZOOM_MAX 4
int ZOOM_LEVEL = ZOOM_MAX;

camera *create_camera() {
  camera *cam = (camera *) malloc(sizeof(camera));
  assert(cam != NULL);
  cam->vector = gsl_vector_calloc(3);
  set_camera_position(cam, 0, 0);
  return cam;
}

void move_camera(camera *camera, int x, int y) {
  set_camera_position(camera, gsl_vector_get(camera->vector, 0) + x, gsl_vector_get(camera->vector, 1) + y);
}

void set_camera_position(camera *camera, int x, int y) {
  int rel_map_size = MAP_SIZE / ZOOM_LEVEL;

  if(rel_map_size < WIDTH) {
    x = - (WIDTH - rel_map_size) / 2;
  } else if(x < 0) {
    x = 0;
  } else if(x + WIDTH >= rel_map_size) {
    x = rel_map_size - WIDTH;
  }

  if(rel_map_size < HEIGHT) {
    y = - (HEIGHT - rel_map_size) / 2;
  } else if(y < 0) {
    y = 0;
  } else if(y + HEIGHT >= rel_map_size) {
    y = rel_map_size - HEIGHT;
  }

  gsl_vector_set(camera->vector, 0, x);
  gsl_vector_set(camera->vector, 1, y);
  gsl_vector_set(camera->vector, 2, 0);
}

void zoom_in(camera *camera) {
  if(ZOOM_LEVEL > ZOOM_MIN) {
    ZOOM_LEVEL -= 1;
    int zoom_diff = HALF_MAP_SIZE * ( 1.0f / (ZOOM_LEVEL * (ZOOM_LEVEL + 1)));
    move_camera(camera, zoom_diff, zoom_diff);
  }
}

void zoom_out(camera *camera) {
  if(ZOOM_LEVEL < ZOOM_MAX) {
    ZOOM_LEVEL += 1;
    int zoom_diff = HALF_MAP_SIZE * ( -1.0f / (ZOOM_LEVEL * (ZOOM_LEVEL - 1)));
    move_camera(camera, zoom_diff, zoom_diff);
  }
}

void print_camera(camera *camera) {
  printf("camera (%f, %f)\n", gsl_vector_get(camera->vector, 0), gsl_vector_get(camera->vector, 1));
}

void remove_camera(camera *camera) {
  gsl_vector_free(camera->vector);
  free(camera);
}
