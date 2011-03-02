#include "priority_queue.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

pqueue *pqueue_init(int max_size, int (*score)(void *)) {
  assert(max_size > 0);

  pqueue *queue = malloc(sizeof(pqueue));
  assert(queue != NULL);
  
  queue->data = calloc(max_size + 1, sizeof(void *));
  assert(queue->data != NULL);
  queue->data[0] = NULL;

  queue->size = 0;
  queue->max_size = max_size;
  queue->score = score;

  return queue;
}

void pqueue_add(pqueue *queue, void *value) {
  if(pqueue_full(queue)) {
    printf("The priority queue is full (%d/%d)\n", queue->size, queue->max_size);
    assert(!pqueue_full(queue));
  }

  int i;
  for(i = ++queue->size; 
      (queue->data[i / 2] == NULL ? -1 : queue->score(queue->data[i / 2])) > queue->score(value); 
      i /= 2) 
  {
    queue->data[i] = queue->data[i / 2];
  }
  queue->data[i] = value;
}

void *pqueue_pop(pqueue *queue) {
  if(pqueue_empty(queue))
    return NULL;

  void *top = queue->data[1];
  void *last = queue->data[queue->size--];

  int i, child;
  for(i = 1; i * 2 <= queue->size; i = child) {
    child = i * 2;
    if(child != queue->size && 
        queue->score(queue->data[child + 1]) < queue->score(queue->data[child]))
      child++;

    if(queue->score(last) > queue->score(queue->data[child]))
      queue->data[i] = queue->data[child];
    else
      break;
  }

  queue->data[i] = last;
  return top;
}

void *pqueue_peek(pqueue *queue) {
  return queue->data[0];
}

bool pqueue_include(pqueue *queue,  void *value) {
  return false;
}

void pqueue_clear(pqueue *queue) {
}

bool pqueue_empty(pqueue *queue) {
  return queue->size == 0;
}

bool pqueue_full(pqueue *queue) {
  return queue->size == queue->max_size;
}
