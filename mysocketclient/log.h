#ifndef _LOG_H_
#define _LOG_H_
/*
#define NO_LOG_LEVEL    0
#define DEBUG_LEVEL     1
#define INFO_LEVEL      2
#define WARNING_LEVEL   3
#define ERROR_LEVEL     4
*/

extern int LogLevel[5];
void LOG(   const char *file,
            int line,
            int level,
            int status,
            const char *fmt,
            ...);


#endif