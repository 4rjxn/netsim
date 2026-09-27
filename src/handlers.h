#ifndef HANDLERS_H
#define HANDLERS_H
#include "graph.h"
#include "ui.h"
#pragma once

void exitHandler(Graph **graph, InputCommand *command, char *output_buff);
void helpHandler(Graph **graph, InputCommand *command, char *output_buff);
void newHandler(Graph **graph, InputCommand *command, char *output_buff);
void addeviceHandler(Graph **graph, InputCommand *command, char *output_buff);
void rmdeviceHandler(Graph **graph, InputCommand *command, char *output_buff);
void showHandler(Graph **graph, InputCommand *command, char *output_buff);
void connectHandler(Graph **graph, InputCommand *command, char *output_buff);
void disconnectHandler(Graph **graph, InputCommand *command, char *output_buff);
void transmitHandler(Graph **graph, InputCommand *command, char *output_buff);

#endif
