#include "camera.h"
int ZOOM_LEVEL = 2;

void zoom_out() {
  if(ZOOM_LEVEL < 4) {
    ZOOM_LEVEL += 1;
  }
}

void zoom_in() {
  if(ZOOM_LEVEL > 1) {
    ZOOM_LEVEL -= 1;
  }
}
