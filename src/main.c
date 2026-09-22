/* This is a graph based network simulation for the dsa lab project
 * Iam plannig to use Graph(adjacency list) to represent the network
 * networks are mostly sparse of this approch is better tham matix */

#include "command.h"
#include "graph.h"
#include "ui.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PAYLOAD_SIZE 1024
#define NAME_SIZE 20
#define DESC_SIZE 256

typedef struct Packet Packet;

struct Packet {
  int id;
  int source_id;
  int destination_id;
  char payload[PAYLOAD_SIZE];
};

void printBanner(void) {
  puts("#######################################################################"
       "#\n"
       "#                                                                      "
       "#\n"
       "#                          WELCOME TO NETSIM                           "
       "#\n"
       "#                                                                      "
       "#\n"
       "#                         use: help for help!                          "
       "#\n"
       "#                                                                      "
       "#\n"
       "#######################################################################"
       "#");
}

void clear() { printf("\033[H\033[J"); }

int main() {
  char input[100];
  Graph *g = NULL;
  clear();
  printBanner();
  for (;;) {
    printf("\nnetsim (%d active): ", g == NULL ? 0 : g->device_count);
    fflush(stdout);
    InputStatus input_status = readLine(input, sizeof(input));
    if (input_status == INPUT_EOF) {
      printf("errror\n");
      break;
    }
    const Command *command = findCommand(input);
    if (command == NULL) {
      printf("invalid command.\n");
      continue;
    }
    if (g == NULL && strcmp(command->name, "new") != 0) {
      printf("no network.\n");
      continue;
    }
    command->handler(&g);
  }
}
