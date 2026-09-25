#include "graph.h"
#include "status.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Graph *newGraph(int vertices_count) {
  Graph *g = malloc(sizeof(Graph));
  if (g == NULL) {
    fprintf(stderr, "memmory allocation error: (newGraph)\n");
    exit(1);
  }
  g->vertices_count = vertices_count;
  g->nodes = malloc(vertices_count * sizeof(Device *));
  if (g->nodes == NULL) {
    fprintf(stderr, "memmory allocation error: (nodeAllocation)\n");
    exit(1);
  }
  g->next_slot = 0;
  g->device_count = 0;
  for (int i = 0; i < vertices_count; i++) {
    g->nodes[i] = NULL;
  }
  return g;
}

void resizeGraph(Graph *graph) {
  graph->vertices_count *= 2;
  Device **temp =
      realloc(graph->nodes, (graph->vertices_count * sizeof(Device *)));
  if (temp == NULL) {
    fprintf(stderr, "memmory allocation error: (resizeGraph)\n");
    exit(1);
  }
  for (int i = graph->next_slot; i < graph->vertices_count; i++) {
    temp[i] = NULL;
  }
  graph->nodes = temp;
}

ExecutionStatus addDevice(Graph *graph, Device *device) {
  if (device == NULL)
    return NULL_DEVICE;
  if (graph->next_slot >= graph->vertices_count) {
    resizeGraph(graph);
  }
  graph->nodes[graph->next_slot] = device;
  device->id = graph->next_slot;
  graph->next_slot++;
  graph->device_count++;
  return OK;
}

bool isValidDevice(Graph *graph, int device_id) {
  if (device_id < 0 || device_id >= graph->next_slot) {
    return false;
  }
  if (graph->nodes[device_id] == NULL) {
    return false;
  }
  return true;
}

void freeDevice(Device *device) { free(device); }

int removeNextLink(Device *head, int id) {
  if (head == NULL)
    return 0;
  Device *ptr = head->next;
  Device *previous = head;
  while (ptr != NULL && ptr->id != id) {
    previous = ptr;
    ptr = ptr->next;
  }
  if (ptr == NULL) {
    return 0;
  }
  previous->next = ptr->next;
  freeDevice(ptr);
  return 1;
}

void freeGraph(Graph *graph) {
  if (graph == NULL) {
    return;
  }
  for (int i = 0; i < graph->next_slot; i++) {
    removeDevice(graph, i);
  }
  free(graph->nodes);
  free(graph);
}

ExecutionStatus removeDevice(Graph *graph, int device_id) {
  if (!(isValidDevice(graph, device_id))) {
    return INVALID_DEVICE;
  }
  Device *d = graph->nodes[device_id];
  Device *ptr = d->next;
  while (ptr != NULL) {
    Device *neighbour_device = graph->nodes[ptr->id];
    removeNextLink(neighbour_device, device_id);
    ptr = ptr->next;
  }
  ptr = d->next;
  while (ptr != NULL) {
    Device *temp = ptr;
    ptr = ptr->next;
    freeDevice(temp);
  }
  freeDevice(d);
  graph->nodes[device_id] = NULL;
  graph->device_count--;
  return OK;
}

Device *newDevice(DeviceType type) {
  Device *d = malloc(sizeof(Device));
  if (d == NULL) {
    fprintf(stderr, "memmory allocation error: (newDevice)\n");
    exit(1);
  }
  d->id = -1;
  d->type = type;
  d->next = NULL;
  d->name[0] = '\0';
  return d;
}

ExecutionStatus addConnection(Graph *graph, int src, int dest) {
  if (!(isValidDevice(graph, src) && isValidDevice(graph, dest))) {
    return INVALID_DEVICE;
  }
  if (src == dest) {
    return SELF_LOOP;
  }

  Device *ptr = graph->nodes[src]->next;
  while (ptr != NULL) {
    if (ptr->id == dest) {
      return DUPLICATE_CONNECTION;
    }
    ptr = ptr->next;
  }
  Device *d = newDevice(CARRY);
  d->id = dest;
  d->next = graph->nodes[src]->next;
  graph->nodes[src]->next = d;

  d = newDevice(CARRY);
  d->id = src;
  d->next = graph->nodes[dest]->next;
  graph->nodes[dest]->next = d;
  return OK;
}

ExecutionStatus removeConnection(Graph *graph, int src, int dest) {
  if (src == dest) {
    return SELF_LOOP;
  }
  if (!(isValidDevice(graph, src) && (isValidDevice(graph, dest)))) {
    return INVALID_DEVICE;
  }
  Device *src_device = graph->nodes[src];
  Device *dest_device = graph->nodes[dest];
  removeNextLink(src_device, dest);
  removeNextLink(dest_device, src);
  return OK;
}

const char *deviceTypeToString(DeviceType type) {
  switch (type) {
  case ROUTER:
    return "ROUTER";
  case SWITCH:
    return "SWITCH";
  case COMPUTER:
    return "COMPUTER";
  case CARRY:
    return "CARRY";
  default:
    return "INVALID";
  }
}

void displayNetwork(Graph *graph) {
  if (graph->device_count == 0) {
    printf("empty graph nothing to print.\n");
  }
  for (int i = 0; i < graph->next_slot; i++) {
    if (graph->nodes[i] == NULL) {
      continue;
    }
    printf("[%s] Name: %s Id: %d\n", deviceTypeToString(graph->nodes[i]->type),
           graph->nodes[i]->name, graph->nodes[i]->id);
    Device *next = graph->nodes[i]->next;
    printf("\tConnection:\n");
    int count = 1;
    while (next != NULL) {
      printf("\t[%d] Id: %d Name: %s Type: %s\n", count,
             graph->nodes[next->id]->id, graph->nodes[next->id]->name,
             deviceTypeToString(graph->nodes[next->id]->type));
      next = next->next;
      count++;
    }
  }
}
