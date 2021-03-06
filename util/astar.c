#include "util/astar.h"
#include "util/priority_queue.h"

#include "collision.h"

#include <assert.h>
#include <math.h>

#define HEIGHT_DIFF_MULT 5

float euclidian_distance(ai_node *, ai_node *);
static ll_node *reconstruct_path(ai_node *);
static bool occupied[MAP_SIZE][MAP_SIZE];

float euclidian_distance(ai_node *node1, ai_node *node2) {
  int xdiff = node1->x - node2->x;
  int ydiff = node1->y - node2->y;
  return sqrt((xdiff * xdiff) + (ydiff * ydiff));
}

ll_node *shortest_path(PLAYERS *players, unit *un, gsl_vector *goal) {
  // make sure the nav_mesh has been created
  assert(nodes != NULL);

  memset(occupied, false, MAP_SIZE * MAP_SIZE);
  for(int p = 0; p < players->num; p++) {
    player *pl = players->players[p];
    for(int d = 0; d < pl->num_divisions; d++) {
      division *div = pl->divisions[d];
      for(int u = 0; u < div->size; u++) {
        unit *unit_to_check = div->units[u];
        if(unit_to_check == un)
          continue;

        int x, y;
        x = (int) round(x(unit_to_check->position));
        y = (int) round(y(unit_to_check->position));

        for(int i = x - unit_to_check->collision_radius; i <= x + unit_to_check->collision_radius; i++) {
          for(int j = y - unit_to_check->collision_radius; j <= y + unit_to_check->collision_radius; j++) { 
            occupied[i][j] = true;
          }
        }
      }
    }
  }

  ai_node *beginning = find_closest_node(un->position);
  ai_node *end = find_closest_node(goal);

  if(beginning == NULL) {
    printf("couldn't find a node near (%f, %f)\n", x(un->position), y(un->position));
    return NULL;
  }

  if(end == NULL) {
    printf("couldn't find a node near (%f, %f)\n", x(goal), y(goal));
    return NULL;
  }

  memset(closed_nodes, false, node_max);
  memset(open_nodes, false, node_max);
  pqueue_clear(open_list);

  beginning->g_score = 0;
  beginning->h_score = euclidian_distance(beginning, end);
  beginning->score = beginning->h_score;
  beginning->came_from = NULL;

  pqueue_add(open_list, beginning);
  open_nodes[beginning->idx] = true;

  ai_node *current, *neighbor;
  float tentative_g_score;
  while(!pqueue_empty(open_list)) {
    current = (ai_node *) pqueue_pop(open_list);
    open_nodes[current->idx] = false;

    if(current == end)
      return reconstruct_path(current);

    closed_nodes[current->idx] = true;

    for(int i = 0; i < current->num_edges; i++) {
      neighbor = current->edges[i]->right;
      if(closed_nodes[neighbor->idx]) // || hit_unit(occupied, current->x, current->y, neighbor->x, neighbor->y))
        continue;

      tentative_g_score = current->g_score + euclidian_distance(current, neighbor) + 
        (height_at(neighbor->x, neighbor->y) - height_at(current->x, current->y)) * HEIGHT_DIFF_MULT;

      bool include = open_nodes[neighbor->idx];
      if(!include || tentative_g_score < neighbor->g_score) {
        neighbor->came_from = current;

        neighbor->g_score = tentative_g_score;
        neighbor->h_score = euclidian_distance(neighbor, end);
        neighbor->score = neighbor->g_score + neighbor->h_score;

        if(!include) {
          pqueue_add(open_list, neighbor);
          open_nodes[neighbor->idx] = true;
        }
      }
    }
  }

  printf("Could not find a path from (%d, %d) to (%d, %d)\n", 
      beginning->x, beginning->y,
      end->x, end->y);
  return NULL;
}

static ll_node *reconstruct_path(ai_node *goal) {
  ai_node *came_from; 

  ll_node *beginning = NULL;
  ai_node *node = goal;

  while(node) {
    beginning = ll_add(beginning, node);

    came_from = node->came_from;
    node->came_from = NULL;
    node = came_from;
  }

  return beginning;
}

ai_node *find_closest_node(gsl_vector *vector) {
  ai_node *node = NULL, *chk_node;
  int x, y, dx, dy;
  int x_to_chk, y_to_chk, xdiff, ydiff;
  int distance = NODE_DISTANCE;

  x = x(vector);
  y = y(vector);

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
          //if(hit_unit(occupied, x, y, chk_node->x, chk_node->y)) {
          //  printf("hit unit between %d, %d and %d, %d\n", x, y, chk_node->x, chk_node->y);
          //  continue;
          //}
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
