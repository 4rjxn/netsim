#include "command.h"
#include "handlers.h"
#include <string.h>

const Command commands[] = {
    {.name = "exit",
     .description = "Exits the program.",
     .handler = exitHandler},
    {.name = "help",
     .description = "prints this help menu.",
     .handler = helpHandler},
    {
        .name = "new",
        .description = "creates a new network frees any existing ones.",
        .handler = newHandler,
    },
    {
        .name = "addevice",
        .description = "add a new devices to the network.",
        .handler = addeviceHandler,
    },
    {
        .name = "rmdevice",
        .description = "removes a device form the network.",
        .handler = rmdeviceHandler,
    },
    {
        .name = "show",
        .description = "Prints the network with connections.",
        .handler = showHandler,
    },
    {
        .name = "connect",
        .description = "Connects two devices.",
        .handler = connectHandler,
    },
    {
        .name = "disconnect",
        .description = "Disconnects two devices.",
        .handler = disconnectHandler,
    },
    {
        .name = "transmit",
        .description =
            "Simulates packet transmission and traces path using stack.",
        .handler = transmitHandler,
    },
    {
        .name = "trace",
        .description = "Tracks packet flow from source to destination.",
        .handler = transmitHandler,
    },
};

const size_t command_count = sizeof(commands) / sizeof(commands[0]);

const Command *findCommand(const char *input) {
  for (size_t i = 0; i < command_count; i++) {
    if (strcmp(commands[i].name, input) == 0) {
      return &commands[i];
    }
  }
  return NULL;
};
