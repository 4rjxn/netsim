/* This is a graph based network simulation for the dsa lab project
 * Iam plannig to use Graph(adjacency list) to represent the network
 * networks are mostly sparse of this approch is better tham matix */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAYLOAD_SIZE 1024
#define NAME_SIZE 20

typedef struct Graph Graph;
typedef struct Device Device;
typedef struct Packet Packet;
typedef enum DeviceType DeviceType;

enum DeviceType { ROUTER, SWITCH, COMPUTER, CARRY };

struct Graph {
  int vertices_count;
  int next_slot;
  int device_count;
  Device **nodes;
};

struct Packet {
  int id;
  int source_id;
  int destination_id;
  char payload[PAYLOAD_SIZE];
};

struct Device {
  int id;
  DeviceType type;
  char name[NAME_SIZE];
  Device *next;
};

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

void freeGraph(Graph *graph) {
  for (int i = 0; i < graph->vertices_count; i++) {
    free(graph->nodes[i]);
  }
  free(graph->nodes);
  free(graph);
}

int addDevice(Graph *graph, Device *device) {
  if (graph->next_slot >= graph->vertices_count) {
    printf("no space in graph resizing\n");
    resizeGraph(graph);
  }
  graph->nodes[graph->next_slot] = device;
  device->id = graph->next_slot;
  graph->next_slot++;
  graph->device_count++;
  return device->id;
}
void freeDevice(Device *device) { free(device); }

void removeDevice(Graph *graph, int device_id) {
  Device *d = graph->nodes[device_id];
  Device *ptr = d->next;
  while (ptr != NULL) {
    Device *child_ptr = graph->nodes[ptr->id];
    while (child_ptr->next->id != device_id) {
      child_ptr = child_ptr->next;
    }
    Device *temp = child_ptr->next;
    child_ptr->next = child_ptr->next->next;
    freeDevice(temp);
    ptr = ptr->next;
  }
  ptr = d->next;
  Device *tmp;
  while (ptr != NULL) {
    tmp = ptr;
    ptr = ptr->next;
    freeDevice(tmp);
  }
  freeDevice(d);
  graph->nodes[device_id] = NULL;
  graph->device_count--;
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
  return d;
}

void addConnection(Graph *graph, int src, int dest) {
  if (src == dest) {
    printf("warning self loops are not allowed\n");
    return;
  }
  Device *d = newDevice(CARRY);
  d->id = dest;
  d->next = graph->nodes[src]->next;
  graph->nodes[src]->next = d;

  d = newDevice(CARRY);
  d->id = src;
  d->next = graph->nodes[dest]->next;
  graph->nodes[dest]->next = d;
}

void removeConnection(Graph *graph, int src, int dest) {}
