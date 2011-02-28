#include "nav_mesh.h"

#include "SDL_gfxPrimitives.h"

#include <assert.h>


void expand_node(ai_node *);
void connect_node(ai_node *, ai_node *);
ai_node *find_or_create_node(int);

static int nodes_per, node_max;

void walk_terrain() {
  nodes_per = MAP_SIZE / NODE_DISTANCE;
  node_max = nodes_per * nodes_per;

  nodes = (walkable_nodes *) malloc(sizeof(walkable_nodes));
  nodes->nodes = (ai_node **) calloc(node_max, sizeof(ai_node *));
  nodes->num = node_max; 

  ai_node *node;

  int index = 0;
  while(index < nodes->num) {
    node = find_or_create_node(index);
    expand_node(node);
    index++;
  }

}

int directions_to_check[][2] = {
  {1, 1}, {1, 0}, {0, 1},
  {-1, -1}, {-1, 0}, {0, -1},
  {-1, 1}, {1, -1}
};

void expand_node(ai_node *node) {
  ai_node *expanded_node;
  int *direction;
  for(int i = 0; i < 9; i++) {
    direction = directions_to_check[i];

    if(direction[0] < 0 && node->x <= abs(direction[0]) * NODE_DISTANCE)
      continue;
    else if(direction[0] > 0 && node->x >= MAP_SIZE - direction[0] * NODE_DISTANCE)
      continue;

    if(direction[1] < 0 && node->y <= abs(direction[1]) * NODE_DISTANCE)
      continue;
    else if(direction[1] > 0 && node->y >= MAP_SIZE - direction[1] * NODE_DISTANCE)
      continue;

    int index = node->idx + direction[0] * nodes_per + direction[1]; 
    if(index < 0 || index >= node_max)
      continue;

    expanded_node = find_or_create_node(index);

    if(height_at(expanded_node->x, expanded_node->y) * 255 > WATER + 5) {
      connect_node(node, expanded_node);
    }
  }
}

void connect_node(ai_node *left, ai_node *right) {
  ai_edge *edge;

  edge = (ai_edge *) malloc(sizeof(ai_edge));
  assert(edge != NULL);
  edge->left = left;
  edge->right = right;

  left->edges[left->num_edges++] = edge;
  assert(left->num_edges < 9);
}

ai_node *find_or_create_node(int index) {
  ai_node *node;

  if(nodes->nodes[index] != NULL) {
    node = nodes->nodes[index];
  } else {
    node = (ai_node *) malloc(sizeof(node));
    assert(node != NULL);
    node->x = index / nodes_per * NODE_DISTANCE;
    node->y = index % nodes_per * NODE_DISTANCE;
    node->idx = index;
    node->num_edges = 0;
    node->edges = (ai_edge **) calloc(8, sizeof(ai_edge *));
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
