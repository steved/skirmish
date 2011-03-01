#include "astar.h"

#include <math.h>

ai_node **shortest_path(gsl_vector *start, gsl_vector *goal) {
  ai_node *beginning = find_closest_node(start);
  return NULL;
}

ai_node *find_closest_node(gsl_vector *vector) {
  ai_node *node = NULL, *chk_node;
  int x, y, dx, dy;
  int x_to_chk, y_to_chk, xdiff, ydiff;
  int distance = NODE_DISTANCE;

  x = gsl_vector_get(vector, 0);
  y = gsl_vector_get(vector, 1);

  float closest_distance = -1, distance_to;

  int node_distance_sq = NODE_DISTANCE * NODE_DISTANCE;
  while(node == NULL && distance <= node_distance_sq) {
    dx = x % distance;
    dy = y % distance;

    for(int i = 1; i >= -1; i -= 2) {
      for(int j = 1; j >= -1; j -= 2) {
        x_to_chk = x;
        if(i == 1)
          x_to_chk += distance - dx;
        else
          x_to_chk -= dx;

        y_to_chk = y;
        if(j == 1)
          y_to_chk += distance - dy;
        else
          y_to_chk -= dy;

        if(x_to_chk > MAP_SIZE || x_to_chk < 0 || y_to_chk > MAP_SIZE || y_to_chk < 0)
          continue;

        chk_node = node_at(x_to_chk, y_to_chk);

        if(chk_node->num_edges > 0 && !hit_water(x, y, chk_node->x, chk_node->y)) {
          xdiff = chk_node->x - x;
          ydiff = chk_node->y - y;
          distance_to = sqrt(xdiff*xdiff + ydiff*ydiff);
          if(closest_distance < 0 || distance_to < closest_distance) {
            closest_distance = distance_to;
            node = chk_node;
          }
        }
      }
    }

    distance += NODE_DISTANCE;
  }

  return node;
}
