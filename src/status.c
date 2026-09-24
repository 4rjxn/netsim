#include "status.h"

const char *statusToString(ExecutionStatus status) {
  switch (status) {
  case OK:
    return "operation performed.";
  case INVALID_DEVICE:
    return "The given device is invalid.";
  case INVALID_ID:
    return "The given id is invalid.";
  case SELF_LOOP:
    return "Self loop is not allowed.";
  case DUPLICATE_CONNECTION:
    return "Duplicate connection is not allowed.";
  case NO_SUCH_CONNECTION:
    return "No such connection.";
  case NULL_DEVICE:
    return "The device is empty.";
  }
}
