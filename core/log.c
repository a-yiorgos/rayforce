#include "log.h"
#include "def.h"
#include "os.h"
#include "string.h"
#include "format.h"

// Default log level if not specified
static log_level_t current_level = LOG_LEVEL_INFO;

// Get log level from environment variable
log_level_t log_get_level(nil_t) {
    c8_t level_str[32];
    if (os_get_var("RAYFORCE_LOG_LEVEL", level_str, sizeof(level_str)) != 0) {
        return current_level;
    }

    if (strcmp(level_str, "TRACE") == 0)
        return LOG_LEVEL_TRACE;
    if (strcmp(level_str, "DEBUG") == 0)
        return LOG_LEVEL_DEBUG;
    if (strcmp(level_str, "INFO") == 0)
        return LOG_LEVEL_INFO;
    if (strcmp(level_str, "WARN") == 0)
        return LOG_LEVEL_WARN;
    if (strcmp(level_str, "ERROR") == 0)
        return LOG_LEVEL_ERROR;
    if (strcmp(level_str, "OFF") == 0)
        return LOG_LEVEL_OFF;

    return current_level;
}

// Get color for log level
static lit_p get_level_color(log_level_t level) {
    switch (level) {
        case LOG_LEVEL_TRACE:
            return CYAN;
        case LOG_LEVEL_DEBUG:
            return BLUE;
        case LOG_LEVEL_INFO:
            return GREEN;
        case LOG_LEVEL_WARN:
            return YELLOW;
        case LOG_LEVEL_ERROR:
            return RED;
        default:
            return RESET;
    }
}

// Get level name
static lit_p get_level_name(log_level_t level) {
    switch (level) {
        case LOG_LEVEL_TRACE:
            return "TRACE";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

// Internal logging function
nil_t log_internal(log_level_t level, lit_p file, i32_t line, lit_p func, lit_p fmt, ...) {
    if (level < log_get_level()) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    // Get timestamp
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Get just the filename from the full path
    const char* filename = strrchr(file, '/');
    filename = filename ? filename + 1 : file;

    // Print the log message with color
    fprintf(stderr, "%s[%s] %s%s:%d %s()%s - %s%s%s - %s", get_level_color(level), timestamp, LIGHT_BLUE, filename,
            line, func, RESET, get_level_color(level), get_level_name(level), RESET, get_level_color(level));
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "%s\n", RESET);

    va_end(args);
}