#ifndef STATUS_H
#define STATUS_H
#pragma once

enum ExecutionStatus {
  OK,
  INVALID_DEVICE,
  SELF_LOOP,
  DUPLICATE_CONNECTION,
  NO_SUCH_CONNECTION,
  NULL_DEVICE
};

typedef enum ExecutionStatus ExecutionStatus;

#endif
