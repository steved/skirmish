#ifndef SKIRMISH_LL
#define SKIRMISH_LL

#include <stdbool.h>

typedef struct ll_node {
  void *value;
  struct ll_node *next;
} ll_node;

ll_node *ll_init(void *);
ll_node *ll_add(ll_node *, void *);
ll_node *ll_remove(ll_node *, void *);
bool ll_include(ll_node *,  void *);
ll_node *ll_clear(ll_node *);

#endif
