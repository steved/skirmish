#include "priority_queue.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void swap(void **, int, int);
static void heapify(pqueue *, int);

pqueue *pqueue_init(int max_size, int (*score)(void *)) {
  assert(max_size > 0);

  pqueue *queue = malloc(sizeof(pqueue));
  assert(queue != NULL);
  
  queue->data = calloc(max_size + 1, sizeof(void *));
  assert(queue->data != NULL);

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
      i > 1 && queue->score(queue->data[i / 2]) > queue->score(value); 
      i /= 2) 
  {
    swap(queue->data, i, i / 2);
  }
  queue->data[i] = value;
}

void *pqueue_pop(pqueue *queue) {
  if(pqueue_empty(queue))
    return NULL;

  void *top = queue->data[1];
  void *last = queue->data[queue->size--];

  queue->data[1] = last;
  heapify(queue, 1);
  return top;
}

void *pqueue_peek(pqueue *queue) {
  return queue->data[0];
}

bool pqueue_include(pqueue *queue,  void *value) {
  return false;
}

void pqueue_clear(pqueue *queue) {
  memset(queue->data, 0, queue->max_size + 1);
  queue->size = 0;
}

bool pqueue_empty(pqueue *queue) {
  return queue->size == 0;
}

bool pqueue_full(pqueue *queue) {
  return queue->size == queue->max_size;
}

static void swap(void **data, int index, int swap) {
  void *temp = data[swap];
  data[swap] = data[index];
  data[index] = temp;
}

static void heapify(pqueue *queue, int index) {
  int left = 2 * index;
  int right = 2 * index + 1;
  int largest;

  if(left <= queue->size && queue->score(queue->data[left]) < queue->score(queue->data[index]))
    largest = left;
  else
    largest = index;

  if(right <= queue->size && queue->score(queue->data[right]) < queue->score(queue->data[largest]))
    largest = right;

  if(largest != index) {
    swap(queue->data, index, largest);
    heapify(queue, largest);
  }
}

void pqueue_free(pqueue *queue) {
  free(queue->data);
  free(queue);

  queue->data = NULL;
  queue = NULL;
}
#if 0
/* gcc -o pqueue priority_queue.c; ./pqueue */

int score(void *);

int main() {
  srand(time(NULL));

  pqueue *queue = pqueue_init(100, &score);
  int *i, j;
  for(j = 0; j < 100; j++) {
    i = malloc(sizeof(int));
    *i = rand() % 1000;
    pqueue_add(queue, i);
  }

  while(!pqueue_empty(queue)) {
    i = pqueue_pop(queue);
    printf("%d\n", *i);
    free(i);
  }
}

int score(void *data) {
  return *((int *) data);
}
#endif
