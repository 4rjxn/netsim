#include "stack.h"
#include <stdlib.h>

Node *newNode() { return malloc(sizeof(Node)); }

void push(Node *tos, Device *device) {
  Node *n = newNode();
  n->data = device;
  n->next = NULL;
  if (tos == NULL) {
    tos = n;
    return;
  }
  n->next = tos;
  tos = n;
}

Device *pop(Node *tos) {
  if (tos == NULL) {
    return NULL;
  }
  Device *device = tos->data;
  Node *tmp = tos;
  tos = tos->next;
  free(tmp);
  return device;
}
