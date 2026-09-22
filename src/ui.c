#include "ui.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prompt(const char *value) {
  printf("%s", value);
  fflush(stdout);
}

void trim(char *str) {
  char *start = str;
  while (isspace((unsigned char)*start) && *start != '\0') {
    start++;
  }
  if (*start == '\0') {
    *str = '\0';
    return;
  }
  char *end = start + strlen(start) - 1;
  while (end > start && isspace((unsigned char)*end)) {
    end--;
  }
  *(end + 1) = '\0';

  memmove(str, start, strlen(start) + 1);
}

InputStatus readLine(char *buf, size_t size) {
  char *res = fgets(buf, size, stdin);
  if (res == NULL) {
    return INPUT_EOF;
  }
  char *c;
  if ((c = strchr(buf, '\n')) != NULL) {
    *c = '\0';
  } else {
    int ch;
    while ((ch = getc(stdin)) != EOF && ch != '\n') {
    }
  }
  return INPUT_OK;
}

InputStatus readInt(int *read_value) {
  char buf[20];
  InputStatus st = readLine(buf, sizeof(buf));
  if (st != INPUT_OK) {
    return st;
  }
  char *end;
  errno = 0;
  long res = strtol(buf, &end, 10);
  if (end == buf || *end != '\0' || errno == ERANGE ||
      !(res <= INT_MAX && res >= INT_MIN)) {
    return INPUT_INVALID;
  }
  *read_value = res;
  return INPUT_OK;
}

InputStatus readName(char *read_value, size_t size) {
  char buf[64];
  InputStatus st = readLine(buf, sizeof(buf));
  if (st != INPUT_OK) {
    return st;
  }
  trim(buf);
  if (*buf == '\0' || strlen(buf) >= size) {
    return INPUT_INVALID;
  }
  strcpy(read_value, buf);
  return INPUT_OK;
}

InputStatus readDeviceType(DeviceType *type) {
  printf("Choose the device type:\n");
  printf("\t1. Router\n");
  printf("\t2. Switch\n");
  printf("\t3. Computer\n");
  prompt("choice(1..3): ");
  int read_int;
  InputStatus res = readInt(&read_int);
  if (res != INPUT_OK) {
    return res;
  }
  switch (read_int) {
  case 1:
    *type = ROUTER;
    return INPUT_OK;
  case 2:
    *type = SWITCH;
    return INPUT_OK;
  case 3:
    *type = COMPUTER;
    return INPUT_OK;
  default:
    return INPUT_INVALID;
  }
}
