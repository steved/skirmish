#ifndef SKIRMISH_CAMERA
#define SKIRMISH_CAMERA

#include <gsl/gsl_vector.h>

extern int ZOOM_LEVEL;

typedef struct camera {
  gsl_vector *vector;
} camera;

camera *create_camera();
void move_camera(camera *, int, int);
void set_camera_position(camera *, int, int);
void zoom_out(camera *);
void zoom_in(camera *);
void print_camera(camera *);
void remove_camera(camera *);

#endif
