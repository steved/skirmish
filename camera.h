#ifndef SKIRMISH_CAMERA
#define SKIRMISH_CAMERA

#include <stdlib.h>
#include <stdio.h>
#include "attributes.h"

extern int ZOOM_LEVEL;

typedef struct camera {
  gsl_vector *vector;
} camera;

camera *create_camera();
void move_camera(camera *camera, int x, int y);
void set_camera_position(camera *camera, int x, int y);
void zoom_out();
void zoom_in();
void print_camera(camera *camera);
void remove_camera(camera *camera);

#endif
