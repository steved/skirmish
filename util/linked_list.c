#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static ll_node *alloc_node();

ll_node *ll_init(void *value) {
  ll_node *node = alloc_node();
  node->value = value;
  node->next = NULL;

  return node;
}

ll_node *ll_add(ll_node *head, void *value) {
  ll_node *node = ll_init(value);
  if(head != NULL)
    node->next = head;
  return node;
}

ll_node *ll_remove(ll_node *head, void *value) {
  if(head == NULL)
    return NULL;

  ll_node *prev = NULL;
  ll_node *node = head;

  while(node) {
    if(node->value == value) {
      if(prev == NULL) 
        head = node->next;
      else
        prev->next = node->next;
      
      free(node);
      return head;
    }

    prev = node;
    node = node->next;
  }

  return head;
}

static ll_node *alloc_node() {
  ll_node *node = malloc(sizeof(ll_node));
  assert(node != NULL);
  return node;
}

ll_node *ll_clear(ll_node *head) {
  if(head == NULL)
    return NULL;

  ll_node *node = head;
  ll_node *next;

  while(node) {
    next = node->next;
    free(node);
    node = next;
  }

  free(node);
  return NULL;
}

bool ll_include(ll_node *head, void *value) {
  ll_node *node = head;
  while(node) {
    if(node->value == value)
      return true;
    node = node->next;
  }
  return false;
}
