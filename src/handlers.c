#include "handlers.h"
#include "command.h"
#include "graph.h"
#include "status.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputStatus readSrcAndDest(int *src, int *dest, InputCommand *command) {
  InputStatus inp_status;
  if (!(*command->arg1 != '\0' &&
        StringtoInt(command->arg1, src) == INPUT_OK)) {
    prompt("\tEnter src id: ");
    inp_status = readInt(src);
    if (inp_status != INPUT_OK) {
      return INPUT_INVALID;
    }
  }
  if (!(*command->arg2 != '\0' &&
        StringtoInt(command->arg2, dest) == INPUT_OK)) {
    prompt("\tEnter dest id: ");
    inp_status = readInt(dest);
    if (inp_status != INPUT_OK) {
      return INPUT_INVALID;
    }
  }
  return INPUT_OK;
}

void exitHandler(Graph **graph, InputCommand *command, char *output_buff) {
  (void)command;
  freeGraph(*graph);
  fillBuffer(output_buff, "bye.\n");
  exit(0);
}

void helpHandler(Graph **graph, InputCommand *command, char *output_buff) {
  (void)command;
  (void)graph;
  fillBuffer(output_buff, "Netsim Helper.\n");
  char buf[OUTPUT_BUFF_SIZE];
  for (size_t i = 0; i < command_count; i++) {
    snprintf(buf, OUTPUT_BUFF_SIZE, "Command: %s\n\tUsage: %s\n",
             commands[i].name, commands[i].description);
    appendToBuffer(output_buff, buf);
  }
}

void newHandler(Graph **graph, InputCommand *command, char *output_buff) {
  (void)command;
  if (graph != NULL) {
    freeGraph(*graph);
  }
  *graph = newGraph(5);
  fillBuffer(output_buff, "created new graph.\n");
}

void addeviceHandler(Graph **graph, InputCommand *command, char *output_buff) {
  (void)command;
  DeviceType type;
  char name[20];
  InputStatus res = readDeviceType(&type);
  if (res != INPUT_OK) {
    fillBuffer(output_buff, "input error check your input.\n");
    return;
  }
  prompt("Give the device a name: ");
  res = readName(name, 20);
  if (res != INPUT_OK) {
    fillBuffer(output_buff, "input error check your input.\n");
    return;
  }
  Device *device = newDevice(type);
  strcpy(device->name, name);
  ExecutionStatus status = addDevice(*graph, device);
  if (status != OK) {
    fillBuffer(output_buff, "cannot complete add device action.\n");
  } else {
    snprintf(output_buff, OUTPUT_BUFF_SIZE,
             "successfuly added the device [ %s ] to the network\n",
             device->name);
  }
}

void rmdeviceHandler(Graph **graph, InputCommand *command, char *output_buff) {
  int device_id;
  if (!(*command->arg1 != '\0' &&
        StringtoInt(command->arg1, &device_id) == INPUT_OK)) {
    prompt("\tEnter the id of the device: ");
    InputStatus inp_status = readInt(&device_id);
    if (inp_status != INPUT_OK) {
      fillBuffer(output_buff, "invalid input\n");
      return;
    }
  }
  char name[NAME_SIZE];

  if ((*graph)->nodes[device_id] == NULL) {
    fillBuffer(output_buff, "invalid device id\n");
    return;
  }
  strcpy(name, (*graph)->nodes[device_id]->name);
  ExecutionStatus status = removeDevice(*graph, device_id);
  if (status == INVALID_DEVICE) {
    fillBuffer(output_buff, "cannot find the device.");
    return;
  }
  snprintf(output_buff, OUTPUT_BUFF_SIZE,
           "removed the device [%s] from the network\n", name);
}

void connectHandler(Graph **graph, InputCommand *command, char *output_buff) {
  int src, dest;
  if (readSrcAndDest(&src, &dest, command) != INPUT_OK) {
    fillBuffer(output_buff, "invalid input\n");
    return;
  }
  ExecutionStatus status = addConnection(*graph, src, dest);
  if (status == SELF_LOOP) {
    fillBuffer(output_buff, "self loop found.\n");
  } else if (status == INVALID_DEVICE) {
    fillBuffer(output_buff, "no device with the id.\n");
  } else if (status == DUPLICATE_CONNECTION) {
    fillBuffer(output_buff, "duplicate connection dectected.\n");
  } else {
    snprintf(output_buff, OUTPUT_BUFF_SIZE, "connected [%s] and [%s]",
             (*graph)->nodes[src]->name, (*graph)->nodes[dest]->name);
  }
}

void disconnectHandler(Graph **graph, InputCommand *command,
                       char *output_buff) {
  int src, dest;
  if (readSrcAndDest(&src, &dest, command) != INPUT_OK) {
    fillBuffer(output_buff, "invalid input\n");
    return;
  }
  ExecutionStatus status = removeConnection(*graph, src, dest);
  if (status == SELF_LOOP) {
    fillBuffer(output_buff, "self loop found.\n");
  } else if (status == INVALID_DEVICE) {
    fillBuffer(output_buff, "no device with the id.\n");
  } else if (status == DUPLICATE_CONNECTION) {
    fillBuffer(output_buff, "duplicate connection dectected.\n");
  } else {
    snprintf(output_buff, OUTPUT_BUFF_SIZE, "disconnected [%s] and [%s]",
             (*graph)->nodes[src]->name, (*graph)->nodes[dest]->name);
  }
}

void showHandler(Graph **graph, InputCommand *command, char *output_buff) {
  (void)command;
  displayNetwork(*graph, output_buff);
}
