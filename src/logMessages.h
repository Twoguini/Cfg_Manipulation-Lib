#ifndef LOGMESSAGES_H
#define LOGMESSAGES_H

#include <stdarg.h>

#define COLOR_INFO "\x1b[34m" // Blue
#define COLOR_WARNING "\x1b[33m" // Yellow
#define COLOR_ERROR "\x1b[31m" // Red
#define COLOR_SUCCESS "\x1b[32m" // Green
#define COLOR_RESET "\x1b[0m"

#define LOG_INFO(fmt, ...) logMessage("INFO", COLOR_INFO, fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) logMessage("WARNING", COLOR_WARNING, fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logMessage("ERROR", COLOR_ERROR, fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_SUCCESS(fmt, ...) logMessage("SUCCESS", COLOR_SUCCESS, fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

void logMessage(const char *level, const char *color, const char *fmt, const char *file, int line, const char *func, ...);

#endif