/* This is a graph based network simulation for the dsa lab project
 * Iam plannig to use Graph(adjacency list) to represent the network
 * networks are mostly sparse of this approch is better tham matix */

#include "graph.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PAYLOAD_SIZE 1024
#define NAME_SIZE 20

typedef struct Graph Graph;
typedef struct Device Device;
typedef struct Packet Packet;
typedef enum DeviceType DeviceType;

struct Packet {
  int id;
  int source_id;
  int destination_id;
  char payload[PAYLOAD_SIZE];
};

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
  addConnection(g, 1, 0);
  addConnection(g, 2, 2);
  addConnection(g, 0, 2);
  displayNetwork(g);
  return 0;
}
