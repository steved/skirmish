#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static ll_node *alloc_node();

ll_node *ll_init(void *value) {
  ll_node *node = alloc_node();
  node->value = value;
  node->next = NULL;
  node->prev = NULL;

  return node;
}

ll_node *ll_add(ll_node *head, void *value) {
  ll_node *node = ll_init(value);
  if(head != NULL) {
    node->next = head;
    head->prev = node;
  }
  return node;
}

ll_node *ll_add_to_bottom(ll_node *head, void *value, void *extra_data) {
  ll_node *node;

  if(head == NULL) {
    node = ll_init(value);
    node->extra_data = extra_data;
    return node;
  } else {
    node = head;

    while(node->next != NULL) {
      node = node->next;
    }

    node->next = ll_init(value);
    node->next->extra_data = extra_data;
    return head;
  }
}

ll_node *ll_remove(ll_node *head, void *value) {
  if(head == NULL)
    return NULL;

  ll_node *node = head;

  while(node) {
    if(node->value == value) {
      if(node->prev == NULL) 
        head = node->next;
      else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
      }

      
      free(node);
      return head;
    }

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
