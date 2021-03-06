#include "log.h"

#include <stdio.h>
#include <stdarg.h>

#define MAX_MSG_LEN 100

static loglevel_t level_config = LOG_NOTSET;
static log_callback_t log_callback;
static uint16_t max_len = MAX_MSG_LEN;


retcode_t log_init (loglevel_t level){
    level_config = level;
    log_callback = (log_callback_t) NULL;
    max_len = MAX_MSG_LEN;
    return RET_OK;
}

retcode_t log_setLevel (loglevel_t level){
    level_config = level;
    return RET_OK;
}

loglevel_t log_getLevel (void){
    return level_config;
}


retcode_t log_setCallback (log_callback_t func){
    if (func == NULL){
        return RET_ERROR;
    }
    log_callback = func;
    return RET_OK;
}


log_callback_t log_getCallback (void){
    return log_callback;
}

retcode_t log_print (loglevel_t msg_lvl, const char * format, ...){
    if (msg_lvl < level_config){
        return RET_WRONG_PARAM;
    }

    if (log_callback == NULL){
        return RET_ERROR;
    }

    char dest[max_len + 1];
    uint16_t msg_len = 0;
    va_list argptr;
    va_start(argptr, format);
    msg_len = (uint16_t) vsnprintf(dest, (size_t)max_len + 1, format, argptr);
    va_end(argptr);

    if (msg_len > max_len){
        msg_len = max_len;
    }
    
    retcode_t ret = log_callback((const char*) dest, msg_len);
    if (ret != RET_OK){
        return RET_RESOURCE_UNAVAILABLE;
    }
    
    return RET_OK;
}


void log_setMaxLen (uint16_t length){
    max_len = length;
}