#include "logMessages.h"

#include <stdio.h>
#include <stdlib.h>

void logMessage(const char *level, const char *color, const char *fmt, const char *file, int line, const char *func, ...) {
  
  va_list args;
  va_start(args, func);

  char message[1024]; 
  vsnprintf(message, sizeof(message), fmt, args);
  
  printf("%s[%s] - %s:%d - (%s) - Message: %s %s\n", color, level, file, line, func, message, COLOR_RESET);

}