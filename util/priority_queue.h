#ifndef SKIRMISH_PQUEUE
#define SKIRMISH_PQUEUE

#include <stdbool.h>

typedef struct priority_queue {
  int (*score)(void *);
  int size, max_size;
  void **data;
} pqueue;

pqueue *pqueue_init(int, int (*score)(void *));
void pqueue_add(pqueue *, void *);
void *pqueue_pop(pqueue *);
bool pqueue_include(pqueue *,  void *);
void pqueue_clear(pqueue *);
bool pqueue_empty(pqueue *);


#endif
