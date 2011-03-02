#include "../collision.h"
#include "nav_mesh.h"

#include "SDL_gfxPrimitives.h"

#include <assert.h>


void expand_node(ai_node *);
void connect_node(ai_node *, ai_node *);
bool in_water(ai_node *);
ai_node *find_or_create_node(int);
walkable_nodes *nodes = NULL;

static int nodes_per;
int node_max = 0;

void walk_terrain() {
  // if regenerating, free the old memory
  if(nodes != NULL)
    free_nav_mesh();

  nodes_per = MAP_SIZE / NODE_DISTANCE + 1;
  node_max = nodes_per * nodes_per;

  nodes = (walkable_nodes *) malloc(sizeof(walkable_nodes));
  nodes->nodes = (ai_node **) calloc(node_max, sizeof(ai_node *));
  nodes->num = node_max; 

  ai_node *node;

  for(int index = 0; index < nodes->num; index++) {
    node = find_or_create_node(index);
    if(in_water(node))
      continue;
    expand_node(node);
  }
}

void expand_node(ai_node *node) {
  ai_node *expanded_node;
  for(int i = 1; i >= -1; i--) {
    for(int j = 1; j >= -1; j--) {
      if(i == 0 && j == 0)
        continue;

      if(i < 0 && node->x == 0)
        continue;
      else if(i > 0 && node->x == MAP_SIZE)
        continue;

      if(j < 0 && node->y == 0)
        continue;
      else if(j > 0 && node->y == MAP_SIZE)
        continue;

      int index = node->idx + i * nodes_per + j; 
      if(index < 0 || index >= node_max)
        continue;

      expanded_node = find_or_create_node(index);
      if(in_water(expanded_node))
        continue;
      connect_node(node, expanded_node);
    }
  }
}

void connect_node(ai_node *left, ai_node *right) {
  if(hit_water(left->x, left->y, right->x, right->y)) {
    return;
  }

  ai_edge *edge;

  edge = (ai_edge *) malloc(sizeof(ai_edge));
  assert(edge != NULL);
  edge->left = left;
  edge->right = right;

  left->edges[left->num_edges++] = edge;
  assert(left->num_edges < 9);
}

ai_node *node_at(int x, int y) {
  int index = y / NODE_DISTANCE + x / NODE_DISTANCE * nodes_per;
  ai_node *node = nodes->nodes[index];
  if(node == NULL) {
    printf("couldn't find node @ (%d, %d) -> %d\n", x, y, index);
    assert(node != NULL);
  }
  return node;
}

ai_node *find_or_create_node(int index) {
  ai_node *node;

  if(nodes->nodes[index] != NULL) {
    node = nodes->nodes[index];
  } else {
    node = (ai_node *) malloc(sizeof(ai_node));
    assert(node != NULL);
    node->x = index / nodes_per * NODE_DISTANCE;
    node->y = index % nodes_per * NODE_DISTANCE;
    node->idx = index;
    node->num_edges = 0;
    node->edges = (ai_edge **) calloc(8, sizeof(ai_edge *));

    node->score = node->g_score = node->h_score = 0;

    assert(node->edges != NULL);
    nodes->nodes[node->idx] = node;
  }

  return node;
}

void draw_nav_mesh(SDL_Surface *surface, bool unconnected, bool edges) {
  for(int i = 0; i < nodes->num; i++) {
    if(!unconnected && nodes->nodes[i]->num_edges == 0)
      continue;

    if(edges) {
      for(int j = 0; j < nodes->nodes[i]->num_edges; j++) {
        lineRGBA(surface, 
            nodes->nodes[i]->edges[j]->left->x, nodes->nodes[i]->edges[j]->left->y,
            nodes->nodes[i]->edges[j]->right->x, nodes->nodes[i]->edges[j]->right->y,
            0, 0, 0xff, 0xff);
      }
    }
    filledCircleRGBA(surface, nodes->nodes[i]->x, nodes->nodes[i]->y, 1, 0xff, 0, 0, 0xff);
  }
}

bool in_water(ai_node *node) {
  return height_at(node->x, node->y) * 255 <= WATER + NODE_WATER_CUTOFF;
}

void free_nav_mesh() {
  if(nodes == NULL)
    return;

  ai_node *node;
  for(int i = 0; i < nodes->num; i++) {
    node = nodes->nodes[i];
    for(int j = 0; j < node->num_edges; j++) {
      free(node->edges[j]);
    }
    free(node->edges);
    free(node);
  }

  free(nodes->nodes);
  free(nodes);
}
