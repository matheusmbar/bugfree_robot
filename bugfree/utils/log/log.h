
#ifndef LOG_H
#define LOG_H

#include "bugfree/retcode.h"

#include <stdint.h>

typedef retcode_t(*log_callback_t)(const char* msg, uint16_t msg_len);

typedef enum {
    LOG_NOTSET = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
}loglevel_t;


retcode_t log_init (loglevel_t level);
retcode_t log_setLevel (loglevel_t level);
loglevel_t log_getLevel (void);
retcode_t log_print (loglevel_t msg_lvl, const char * format, ...);

retcode_t log_setCallback (log_callback_t func);
log_callback_t log_getCallback (void);
void log_setMaxLen (uint16_t length);

#endif
