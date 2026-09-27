#include "bfs.h"
#include "graph.h"
#include "stack.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int findPathBFS(Graph *graph, int src_id, int dest_id, int *path_out,
                       int max_nodes) {
  if (!isValidDevice(graph, src_id) || !isValidDevice(graph, dest_id)) {
    return 0;
  }

  int num_slots = graph->next_slot;
  bool *visited = calloc(num_slots, sizeof(bool));
  int *parent = malloc(num_slots * sizeof(int));
  int *queue = malloc(num_slots * sizeof(int));
  if (!visited || !parent || !queue) {
    free(visited);
    free(parent);
    free(queue);
    return 0;
  }

  for (int i = 0; i < num_slots; i++) {
    parent[i] = -1;
  }

  int front = 0, rear = 0;
  queue[rear++] = src_id;
  visited[src_id] = true;

  bool found = false;
  while (front < rear) {
    int current = queue[front++];
    if (current == dest_id) {
      found = true;
      break;
    }

    Device *edge = graph->nodes[current]->next;
    while (edge != NULL) {
      int neighbor = edge->id;
      if (isValidDevice(graph, neighbor) && !visited[neighbor]) {
        visited[neighbor] = true;
        parent[neighbor] = current;
        queue[rear++] = neighbor;
      }
      edge = edge->next;
    }
  }

  int path_len = 0;
  if (found) {
    int curr = dest_id;
    int *temp_path = malloc(num_slots * sizeof(int));
    int count = 0;
    while (curr != -1) {
      temp_path[count++] = curr;
      curr = parent[curr];
    }
    // Reverse temp_path into path_out
    for (int i = 0; i < count && i < max_nodes; i++) {
      path_out[i] = temp_path[count - 1 - i];
    }
    path_len = count;
    free(temp_path);
  }

  free(visited);
  free(parent);
  free(queue);
  return path_len;
}

ExecutionStatus simulateTransmission(Graph *graph, int src_id, int dest_id,
                                     char *output_buff) {
  if (!isValidDevice(graph, src_id) || !isValidDevice(graph, dest_id)) {
    fillBuffer(output_buff,
               "Error: Invalid source or destination device ID.\n");
    return INVALID_DEVICE;
  }

  if (src_id == dest_id) {
    fillBuffer(output_buff,
               "Notice: Source and destination are the same device.\n");
    return SELF_LOOP;
  }

  int max_nodes = graph->next_slot;
  int *path = malloc(max_nodes * sizeof(int));
  if (!path) {
    fillBuffer(output_buff,
               "Error: Memory allocation failure during path search.\n");
    return NULL_DEVICE;
  }

  int path_len = findPathBFS(graph, src_id, dest_id, path, max_nodes);
  if (path_len == 0) {
    snprintf(output_buff, OUTPUT_BUFF_SIZE,
             "Unreachable: No network path exists between [%s] (ID: %d) and "
             "[%s] (ID: %d).\n",
             graph->nodes[src_id]->name, src_id, graph->nodes[dest_id]->name,
             dest_id);
    free(path);
    return UNREACHABLE;
  }

  clearBuffer(output_buff);
  char line[OUTPUT_BUFF_SIZE];

  snprintf(line, OUTPUT_BUFF_SIZE,
           "=== PACKET TRANSMISSION & ROUTE TRACING ===\n");
  appendToBuffer(output_buff, line);

  snprintf(line, OUTPUT_BUFF_SIZE, "Source:      [%s] %s (ID: %d)\n",
           deviceTypeToString(graph->nodes[src_id]->type),
           graph->nodes[src_id]->name, src_id);
  appendToBuffer(output_buff, line);

  snprintf(line, OUTPUT_BUFF_SIZE, "Destination: [%s] %s (ID: %d)\n\n",
           deviceTypeToString(graph->nodes[dest_id]->type),
           graph->nodes[dest_id]->name, dest_id);
  appendToBuffer(output_buff, line);

  appendToBuffer(output_buff, "--- ROUTE DISCOVERED ---\nPath: ");
  for (int i = 0; i < path_len; i++) {
    Device *d = graph->nodes[path[i]];
    snprintf(line, OUTPUT_BUFF_SIZE, "[%s: %s]%s", deviceTypeToString(d->type),
             d->name, (i == path_len - 1) ? "" : " -> ");
    appendToBuffer(output_buff, line);
  }
  appendToBuffer(output_buff, "\n\n");

  Stack *stack = createStack();

  appendToBuffer(output_buff, "--- TRANSMISSION PHASE (STACK PUSH) ---\n");
  for (int i = 0; i < path_len; i++) {
    Device *d = graph->nodes[path[i]];
    push(stack, d);
    snprintf(
        line, OUTPUT_BUFF_SIZE,
        "[STACK PUSH] Hop %d/%d: Transmitting packet to [%s] %s (ID: %d)\n",
        i + 1, path_len, deviceTypeToString(d->type), d->name, d->id);
    appendToBuffer(output_buff, line);
  }

  appendToBuffer(output_buff,
                 "\n--- RECEPTION & ACKNOWLEDGMENT PHASE (STACK POP) ---\n");
  while (!isStackEmpty(stack)) {
    Device *d = pop(stack);
    snprintf(line, OUTPUT_BUFF_SIZE,
             "[STACK POP]  Processing ACK/Reception at [%s] %s (ID: %d)\n",
             deviceTypeToString(d->type), d->name, d->id);
    appendToBuffer(output_buff, line);
  }

  appendToBuffer(
      output_buff,
      "\n=== TRANSMISSION COMPLETE: Packet successfully delivered! ===\n");

  freeStack(stack);
  free(path);
  return OK;
}
