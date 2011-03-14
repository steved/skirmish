#include "util/linked_list.h"

#include <assert.h>
#include <stdlib.h>

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
  ll_node *node_to_add = ll_init(value);
  node_to_add->extra_data = extra_data;

  if(head == NULL) {
    return node_to_add;
  } else {
    ll_node *node = head;

    while(node->next != NULL) {
      node = node->next;
    }

    node_to_add->prev = node;
    node->next = node_to_add;
    return head;
  }
}

ll_node *ll_remove(ll_node *head, void *value) {
  if(head == NULL)
    return NULL;

  ll_node *node = head;

  while(node) {
    if(node->value == value) {
      if(node == head) 
        head = node->next;
      else {
        node->prev->next = node->next;
        if(node->next)
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


ll_node *ll_copy(ll_node *head) {
  ll_node *copy_head = ll_init(head->value);
  ll_node *prev, *node, *copy;

  node = head->next;
  prev = copy_head;
  while(node) {
    copy = ll_init(node->value);
    prev->next = copy;
    copy->prev = prev;

    prev = copy;
    node = node->next;
  }

  return copy_head;
}
