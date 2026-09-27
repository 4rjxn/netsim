#include "stack.h"
#include <stdlib.h>

Stack *createStack(void) {
  Stack *s = malloc(sizeof(Stack));
  if (s != NULL) {
    s->top = NULL;
    s->size = 0;
  }
  return s;
}

void push(Stack *s, Device *device) {
  if (s == NULL) return;
  Node *n = malloc(sizeof(Node));
  if (n == NULL) return;
  n->data = device;
  n->next = s->top;
  s->top = n;
  s->size++;
}

Device *pop(Stack *s) {
  if (s == NULL || s->top == NULL) return NULL;
  Node *temp = s->top;
  Device *device = temp->data;
  s->top = temp->next;
  free(temp);
  s->size--;
  return device;
}

Device *peek(Stack *s) {
  if (s == NULL || s->top == NULL) return NULL;
  return s->top->data;
}

bool isStackEmpty(Stack *s) {
  return (s == NULL || s->top == NULL);
}

void freeStack(Stack *s) {
  if (s == NULL) return;
  while (!isStackEmpty(s)) {
    pop(s);
  }
  free(s);
}
