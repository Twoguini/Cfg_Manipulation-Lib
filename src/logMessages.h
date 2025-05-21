#ifndef LOGMESSAGES_H
#define LOGMESSAGES_H

#define COLOR_INFO "\x1b[34m" // Blue
#define COLOR_WARNING "\x1b[33m" // Yellow
#define COLOR_ERROR "\x1b[31m" // Red
#define COLOR_SUCCESS "\x1b[32m" // Green
#define COLOR_RESET "\x1b[0m"

#define LOG_INFO(msg) logMessage("INFO", COLOR_INFO, msg, __FILE__, __LINE__, __func__)
#define LOG_WARN(msg) logMessage("WARNING", COLOR_WARNING, msg, __FILE__, __LINE__, __func__)
#define LOG_ERROR(msg) logMessage("ERROR", COLOR_ERROR, msg, __FILE__, __LINE__, __func__)
#define LOG_SUCCESS(msg) logMessage("SUCCESS", COLOR_SUCCESS, msg, __FILE__, __LINE__, __func__)

void logMessage(const char *level, const char *color, const char *msg, const char *file, int line, const char *func);

#endif