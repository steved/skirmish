#ifndef SKIRMISH_LL
#define SKIRMISH_LL

#include <stdbool.h>

typedef struct ll_node {
  void *value;
  void *extra_data; // any extraneous data needed by this node
  struct ll_node *next;
  struct ll_node *prev;
} ll_node;

ll_node *ll_init(void *);
ll_node *ll_add(ll_node *, void *);
ll_node *ll_add_to_bottom(ll_node *, void *, void *);
ll_node *ll_remove(ll_node *, void *);
bool ll_include(ll_node *,  void *);
ll_node *ll_clear(ll_node *);

#endif
