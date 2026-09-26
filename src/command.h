#ifndef COMMAND_H
#define COMMAND_H
#include "graph.h"
#include "ui.h"
#include <stddef.h>
#pragma once

typedef struct Command Command;

struct Command {
  char *name;
  char *description;
  void (*handler)(Graph **graph, InputCommand *command);
};

extern const Command commands[];
extern const size_t command_count;

const Command *findCommand(const char *input);

#endif
