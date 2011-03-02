#ifndef SKIRMISH_NAV_MESH
#define SKIRMISH_NAV_MESH

#include "terrain.h"
#include <stdbool.h>

struct ai_edge;
typedef struct ai_node {
  int idx;
  int x, y;
  int num_edges;

  struct ai_edge **edges;

  int score, g_score, h_score;
} ai_node;

typedef struct ai_edge {
  ai_node *left, *right;
} ai_edge;

typedef struct walkable_nodes {
  ai_node **nodes;
  int num;
} walkable_nodes;

#define NODE_DISTANCE 16
#define NODE_WATER_CUTOFF 0

extern walkable_nodes *nodes;
void walk_terrain();
void draw_nav_mesh(SDL_Surface *, bool, bool);
ai_node *node_at(int, int);
void free_nav_mesh();

#endif
