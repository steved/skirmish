#include "camera.h"

int ZOOM_LEVEL = 2;

camera *create_camera() {
  camera *cam = (camera *) malloc(sizeof(camera));
  cam->vector = gsl_vector_calloc(2);
  set_camera_position(cam, 0, 0);
  return cam;
}

void move_camera(camera *camera, int x, int y) {
  set_camera_position(camera, gsl_vector_get(camera->vector, 0) + x, gsl_vector_get(camera->vector, 1) + y);
}

void set_camera_position(camera *camera, int x, int y) {
  if(x >= 0 && y >= 0) {
    gsl_vector_set(camera->vector, 0, x);
    gsl_vector_set(camera->vector, 1, y);
  }

}

void zoom_out() {
  if(ZOOM_LEVEL > 1) {
    ZOOM_LEVEL -= 1;
  }
}

void zoom_in() {
  if(ZOOM_LEVEL < 4) {
    ZOOM_LEVEL += 1;
  }
}

void print_camera(camera *camera) {
  printf("camera (%f, %f)\n", gsl_vector_get(camera->vector, 0), gsl_vector_get(camera->vector, 1));
}

void remove_camera(camera *camera) {
  gsl_vector_free(camera->vector);
  free(camera);
}
