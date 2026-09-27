#ifndef STACK_H
#define STACK_H
#include "graph.h"
#pragma once

typedef struct Node Node;
struct Node {
  Device *data;
  Node *next;
};

#endif
