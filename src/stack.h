#ifndef STACK_H
#define STACK_H

#include "graph.h"
#include <stdbool.h>

#pragma once

typedef struct Node Node;
struct Node {
  Device *data;
  Node *next;
};

typedef struct {
  Node *top;
  int size;
} Stack;

Stack *createStack(void);
void push(Stack *s, Device *device);
Device *pop(Stack *s);
Device *peek(Stack *s);
bool isStackEmpty(Stack *s);
void freeStack(Stack *s);

#endif
