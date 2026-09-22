#ifndef STATUS_H
#define STATUS_H
#include "graph.h"
#include <stddef.h>
#pragma once

enum InputStatus { INPUT_OK, INPUT_INVALID, INPUT_EOF };
typedef enum InputStatus InputStatus;

InputStatus readLine(char *buf, size_t size);
InputStatus readInt(int *read_value);
InputStatus readName(char *read_value, size_t size);
InputStatus readDeviceType(DeviceType *type);
void prompt(const char *value);

#endif
