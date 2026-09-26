#ifndef HANDLERS_H
#define HANDLERS_H
#include "graph.h"
#include "ui.h"
#pragma once

void exitHandler(Graph **graph, InputCommand *command);
void helpHandler(Graph **graph, InputCommand *command);
void newHandler(Graph **graph, InputCommand *command);
void addeviceHandler(Graph **graph, InputCommand *command);
void rmdeviceHandler(Graph **graph, InputCommand *command);
void showHandler(Graph **graph, InputCommand *command);
void connectHandler(Graph **graph, InputCommand *command);
void disconnectHandler(Graph **graph, InputCommand *command);

#endif
