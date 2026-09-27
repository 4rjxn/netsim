#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "status.h"

ExecutionStatus simulateTransmission(Graph *graph, int src_id, int dest_id,
                                     char *output_buff);

#endif
