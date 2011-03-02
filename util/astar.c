#include "astar.h"
#include "../collision.h"
#include "priority_queue.h"

#include <assert.h>
#include <math.h>

int ai_score(void *);
float euclidian_distance(ai_node *, ai_node *);
static ll_node *reconstruct_path(int *, ai_node *);

int ai_score(void *value) {
  ai_node *node = (ai_node *) value;
  return node->score;
}

float euclidian_distance(ai_node *node1, ai_node *node2) {
  int xdiff = node1->x - node2->x;
  int ydiff = node1->y - node2->y;
  return sqrt((xdiff * xdiff) + (ydiff * ydiff));
}

#define MAX_OPEN_SIZE 100

static int *came_from = NULL;

ll_node *shortest_path(gsl_vector *start, gsl_vector *goal) {
  // make sure the nav_mesh has been created
  assert(node_max > 0);

  // allocate the came_from array if it hasnt' yet
  if(came_from == NULL) {
    came_from = malloc(sizeof(int) * node_max);
    assert(came_from != NULL);
  }
  memset(came_from, -1, node_max);

  ai_node *beginning = find_closest_node(start);
  ai_node *end = find_closest_node(goal);

  if(beginning == NULL) {
    printf("couldn't find a node near (%f, %f)\n", gsl_vector_get(start, 0), gsl_vector_get(start, 1));
    return NULL;
  }

  if(end == NULL) {
    printf("couldn't find a node near (%f, %f)\n", gsl_vector_get(goal, 0), gsl_vector_get(goal, 1));
    return NULL;
  }

  pqueue *open = pqueue_init(MAX_OPEN_SIZE, &ai_score);
  ll_node *closed = NULL;

  beginning->g_score = 0;
  beginning->h_score = euclidian_distance(beginning, end);
  beginning->score = beginning->h_score;

  pqueue_add(open, beginning);

  ai_node *current, *neighbor;
  int tentative_g_score;
  while(!pqueue_empty(open)) {
    current = pqueue_pop(open);
    if(current == end)
      return reconstruct_path(came_from, current);

    closed = ll_add(closed, current);
    for(int i = 0; i < current->num_edges; i++) {
      neighbor = current->edges[i]->right;
      if(ll_include(closed, neighbor))
        continue;

      tentative_g_score = current->g_score + euclidian_distance(current, neighbor);

      bool include = pqueue_include(open, neighbor);
      if(!include || tentative_g_score < neighbor->g_score) {
        if(!include)
          pqueue_add(open, neighbor);
        
        came_from[neighbor->idx] = current->idx;

        neighbor->g_score = tentative_g_score;
        neighbor->h_score = euclidian_distance(neighbor, end);
        neighbor->score = neighbor->g_score + neighbor->h_score;
      }
    }
  }

  printf("Could not find a path from (%d, %d) to (%d, %d)\n", 
      beginning->x, beginning->y,
      end->x, end->y);
  return NULL;
}

static ll_node *reconstruct_path(int *came_from, ai_node *goal) {
  ai_node *node; 

  ll_node *beginning = ll_init(goal);
  int idx = came_from[goal->idx];

  while(idx != -1) {
    node = nodes->nodes[idx];
    beginning = ll_add(beginning, node);
    idx = came_from[idx];
  }

  return beginning;
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
