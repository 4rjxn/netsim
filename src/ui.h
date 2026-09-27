#ifndef UI_H
#define UI_H
#include "graph.h"
#include <stddef.h>
#define OUTPUT_BUFF_SIZE 1024
#pragma once

enum InputStatus { INPUT_OK, INPUT_INVALID, INPUT_EOF };
typedef enum InputStatus InputStatus;
typedef struct InputCommand InputCommand;

struct InputCommand {
  char command[NAME_SIZE];
  char arg1[NAME_SIZE];
  char arg2[NAME_SIZE];
};

InputStatus readLine(char *buf, size_t size);
InputStatus readInt(int *read_value);
InputStatus readName(char *read_value, size_t size);
InputStatus readCommand(InputCommand *command);
InputStatus readDeviceType(DeviceType *type);
InputStatus StringtoInt(char *buf, int *int_value);
void displayNetwork(Graph *graph, char *output_buff);
void fillBuffer(char *buff, char *data);
void appendToBuffer(char *buff, char *data);
void clearBuffer(char *buff);
const char *deviceTypeToString(DeviceType type);
void prompt(const char *value);

#endif
