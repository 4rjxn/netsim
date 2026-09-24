#include "handlers.h"
#include "command.h"
#include "graph.h"
#include "status.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exitHandler(Graph **graph) {
  freeGraph(*graph);
  printf("bye.\n");
  exit(0);
}

void helpHandler(Graph **graph) {
  (void)graph;
  printf("Netsim Helper.\n");
  for (size_t i = 0; i < command_count; i++) {
    printf("Command: %s\n\tUsage: %s\n", commands[i].name,
           commands[i].description);
  }
}

void newHandler(Graph **graph) {
  if (graph != NULL) {
    freeGraph(*graph);
  }
  *graph = newGraph(5);
  printf("created new graph.\n");
}

void addeviceHandler(Graph **graph) {
  DeviceType type;
  char name[20];
  InputStatus res = readDeviceType(&type);
  if (res != INPUT_OK) {
    printf("input error check your input.\n");
    return;
  }
  prompt("Give the device a name: ");
  res = readName(name, 20);
  if (res != INPUT_OK) {
    printf("input error check your input.\n");
    return;
  }
  Device *device = newDevice(type);
  strcpy(device->name, name);
  ExecutionStatus status = addDevice(*graph, device);
  if (status != OK) {
    printf("cannot complete add device action.\n");
  } else {
    printf("successfuly added the device [ %s ] to the network\n",
           device->name);
  }
}

void connectHandler(Graph **graph) {
  int src, dest;
  prompt("\tEnter src id: ");
  readInt(&src);
  prompt("\tEnter dest id: ");
  readInt(&dest);
  ExecutionStatus status = addConnection(*graph, src, dest);
  if (status == SELF_LOOP) {
    printf("self loop found.\n");
  } else if (status == INVALID_DEVICE) {
    printf("no device with the id.\n");
  } else if (status == DUPLICATE_CONNECTION) {
    printf("duplicate connection dectected.\n");
  } else {
    printf("connected [%s] and [%s]", (*graph)->nodes[src]->name,
           (*graph)->nodes[dest]->name);
  }
}

void showHandler(Graph **graph) { displayNetwork(*graph); }
