#include "logMessages.h"

#include <stdio.h>
#include <stdlib.h>

void logMessage(const char *level, const char *color, const char *msg, const char *file, int line, const char *func) {
  printf("%s[%s] - %s:%d - (%s) - Message: %s %s\n", color, level, file, line, func, msg, COLOR_RESET);
}