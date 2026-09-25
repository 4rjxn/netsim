#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED
#include "status.h"
#define NAME_SIZE 20
#pragma once

typedef struct Graph Graph;
typedef struct Device Device;

enum DeviceType { ROUTER, SWITCH, COMPUTER, CARRY };
typedef enum DeviceType DeviceType;

struct Device {
  int id;
  DeviceType type;
  char name[NAME_SIZE];
  Device *next;
};

struct Graph {
  int vertices_count;
  int next_slot;
  int device_count;
  Device **nodes;
};

// creates a new graph with the given vertices count.
Graph *newGraph(int vertices_count);
// frees a complete graph from the memmory.
void freeGraph(Graph *graph);
// addes a new device to the graph
ExecutionStatus addDevice(Graph *graph, Device *device);
// removes a device form a graph
ExecutionStatus removeDevice(Graph *graph, int device_id);
// creates a new device (device name needs to be specified later)
Device *newDevice(DeviceType type);
// addes a connection between two devices
ExecutionStatus addConnection(Graph *graph, int src, int dest);
// removes a connection from two devices
ExecutionStatus removeConnection(Graph *graph, int src, int dest);
// displayes the given network details form the graph
void displayNetwork(Graph *graph);

#endif // !
