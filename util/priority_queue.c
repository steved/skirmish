#include "priority_queue.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define parent(i) (floor(((i) - 1) / 2))

static void heapify(pqueue *, int);
static void swap(void **, int, int);

pqueue *pqueue_init(int max_size, int (*score)(void *)) {
  assert(max_size > 0);

  pqueue *queue = malloc(sizeof(pqueue));
  assert(queue != NULL);
  
  queue->data = calloc(max_size, sizeof(void *));
  assert(queue->data != NULL);

  queue->size = 0;
  queue->max_size = max_size;
  queue->score = score;

  return queue;
}

void pqueue_add(pqueue *queue, void *value) {
  int i = ++queue->size;

  int parent_idx = parent(i);
  while(i > 0 && queue->score(queue->data[parent_idx]) < queue->score(value)) {
    queue->data[i] = queue->data[parent_idx];
    i = parent_idx;
    parent_idx = parent(i);
  }
  queue->data[i] = value;
}

void *pqueue_pop(pqueue *queue) {
  if(pqueue_empty(queue))
    return NULL;

  void *top = queue->data[0];
  queue->data[0] = queue->data[queue->size--];
  heapify(queue, 0);

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

static void heapify(pqueue *queue, int index) {
  int left = 2 * index;
  int right = left + 1;
  int largest = index;

  if(left <= queue->size && queue->score(queue->data[left]) > queue->score(queue->data[index]))
    largest = left;
  if(right <= queue->size && queue->score(queue->data[right]) > queue->score(queue->data[largest]))
    largest = right;

  if(largest != index)
    swap(queue->data, index, largest);

  heapify(queue, largest);
}

static void swap(void **data, int index_a, int index_b) {
  void *temp = data[index_b];
  data[index_b] = data[index_a];
  data[index_a] = temp;
}
