/* This is a graph based network simulation for the dsa lab project
 * Iam plannig to use Graph(adjacency list) to represent the network
 * networks are mostly sparse of this approch is better tham matix */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  for (int i = 0; i < graph->next_slot; i++) {
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

void removeDevice(Graph *graph, int device_id) {
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

void removeConnection(Graph *graph, int src, int dest) {
  Device *src_device = graph->nodes[src];
  Device *dest_device = graph->nodes[dest];
  removeNextLink(src_device, dest);
  removeNextLink(dest_device, src);
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

void displayGraph(Graph *graph) {
  for (int i = 0; i < graph->vertices_count; i++) {
    if (graph->nodes[i] == NULL) {
      continue;
    }
    printf("Device: %s Id: %d\n", graph->nodes[i]->name, graph->nodes[i]->id);
    Device *next = graph->nodes[i]->next;
    printf("\tConnection:\n");
    while (next != NULL) {
      printf("\tId: %d Name: %s Type: %s\n", graph->nodes[next->id]->id,
             graph->nodes[next->id]->name,
             deviceTypeToString(graph->nodes[next->id]->type));
      next = next->next;
    }
  }
}

void showMenu() {
  printf("1. Display network\n");
  printf("2. Add Device\n");
  printf("3. Remove Device\n");
  printf("4. Add Connection\n");
  printf("5. Remove Connection\n");
  printf("> ");
}

int main() {
  // showMenu();
  Graph *g = newGraph(5);
  Device *d = newDevice(ROUTER);
  Device *d2 = newDevice(ROUTER);
  Device *computer = newDevice(COMPUTER);
  strcpy(d->name, "merin");
  strcpy(d2->name, "carlo");
  strcpy(computer->name, "asus-zenbook");
  addDevice(g, d);
  addDevice(g, d2);
  addDevice(g, computer);
  addConnection(g, 0, 1);
  addConnection(g, 2, 0);
  displayGraph(g);
  return 0;
}
